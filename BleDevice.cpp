#include "BleDevice.h"

// constructor
BleDevice::BleDevice(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) 
    : hid(0)
    , deviceName(std::string(deviceName).substr(0, 15))
    , deviceManufacturer(std::string(deviceManufacturer).substr(0,15))
    , batteryLevel(batteryLevel) {}

// methods
void BleDevice::begin(bool _enableMouse, bool _enableKeyboard)
{	
	enableMouse = _enableMouse; enableKeyboard = _enableKeyboard;

	BLEDevice::init(deviceName);
	BLEServer* pServer = BLEDevice::createServer(); // return the same server :( 
	pServer->setCallbacks(this);

	if( enableMouse ){
		hid_mouse = new BLEHIDDevice(pServer);
		inputMouse = hid_mouse->inputReport(MOUSE_ID);  // <-- input REPORTID from report map

		hid_mouse->manufacturer()->setValue(deviceManufacturer);
		hid_mouse->pnp(0x02, vid, pid, version);
		hid_mouse->hidInfo(0x00, 0x02);
	}

	if( enableKeyboard ){
		hid = new BLEHIDDevice(pServer);
		inputKeyboard = hid->inputReport(KEYBOARD_ID);  // <-- input REPORTID from report map
		outputKeyboard = hid->outputReport(KEYBOARD_ID);
		inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);
		outputKeyboard->setCallbacks(this);

		hid->manufacturer()->setValue(deviceManufacturer);
		hid->pnp(0x02, vid, pid, version);
		hid->hidInfo(0x00, 0x01);
	}
	
	BLEDevice::setSecurityAuth(true, true, true);
	// BLESecurity *pSecurity = new NimBLESecurity(); // NimBLE-Mouse does this
  	// pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

	if( enableMouse ){
		hid_mouse->reportMap((uint8_t*)_hidReportDescriptorMouse, sizeof(_hidReportDescriptorMouse));
		//oN ANDROID depend on what start first that will act first
		hid_mouse->startServices();
	}
	if( enableKeyboard ){
		hid->reportMap((uint8_t*)_hidReportDescriptorKeyboard, sizeof(_hidReportDescriptorKeyboard));
		hid->startServices();
	}

	onStarted(pServer);

	BLEAdvertising* advertising = pServer->getAdvertising();
	uint8_t symbol = enableMouse && enableKeyboard ? GENERIC_HID : (enableMouse ? HID_MOUSE : HID_KEYBOARD);
	advertising->setAppearance(symbol); 
	if( enableMouse )
		advertising->addServiceUUID(hid_mouse->hidService()->getUUID());
	if( enableKeyboard )
		advertising->addServiceUUID(hid->hidService()->getUUID());
	//   advertising->setScanResponse(true); // this wont work if you use multiple devices
	advertising->setScanResponse(false);
	//   advertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	//   advertising->setMaxPreferred(0x12);	
	advertising->start();
	
	if( !enableKeyboard ) // attach to mouse if no keyboard, else attach to keyboard
		hid_mouse->setBatteryLevel(batteryLevel);
	else
		hid->setBatteryLevel(batteryLevel);
	// hid_mouse->setBatteryLevel(batteryLevel);

	ESP_LOGD(LOG_TAG, "Advertising started!");
	// vTaskDelay(portMAX_DELAY); //delay(portMAX_DELAY);
}

void BleDevice::end(bool clearAll)
{
	BLEDevice::stopAdvertising();
	BLEServer* pServer = BLEDevice::getServer();
	if (pServer->getConnectedCount() > 0){
		pServer->disconnect(pServer->getPeerIDInfo(0).getConnHandle());
	}
	// delete hid;
	// delete hid_mouse;
	BLEDevice::deinit(clearAll);
}

bool BleDevice::isConnected(void) {
  return this->connected;
}

void BleDevice::setBatteryLevel(uint8_t level) {
  this->batteryLevel = level;
  
  if (hid != 0){
	if( !enableKeyboard )
  		this->hid_mouse->setBatteryLevel(this->batteryLevel);
	else
		this->hid->setBatteryLevel(this->batteryLevel);
  }
    
}

//must be called before begin in order to set the name
void BleDevice::setName(std::string deviceName) {
  this->deviceName = deviceName;
}

/**
 * @brief Sets the waiting time (in milliseconds) between multiple keystrokes in NimBLE mode.
 * 
 * @param ms Time in milliseconds
 */
void BleDevice::setDelay(uint32_t ms) {
  this->_delay_ms = ms;
}

void BleDevice::setVendorID(uint16_t vid) { 
	this->vid = vid; 
}

void BleDevice::setProductID(uint16_t pid) { 
	this->pid = pid; 
}

void BleDevice::setVersion(uint16_t version) { 
	this->version = version; 
}

void BleDevice::sendReport(KeyReport* keys)
{
  if ( connected )
  {
    this->inputKeyboard->setValue((uint8_t*)keys, sizeof(KeyReport));
    this->inputKeyboard->notify();
    this->delay_ms(_delay_ms);
  }	
}

void BleDevice::sendReport(MediaKeyReport* keys)
{
  if ( connected )
  {
    this->inputMediaKeys->setValue((uint8_t*)keys, sizeof(MediaKeyReport));
    this->inputMediaKeys->notify();
    this->delay_ms(_delay_ms);
  }	
}

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t BleDevice::press(uint8_t k)
{
	uint8_t i;
	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	} else if (k >= 128) {	// it's a modifier key
		_keyReport.modifiers |= (1<<(k-128));
		k = 0;
	} else {				// it's a printing key
		k = pgm_read_byte(_asciimap + k);
		if (!k) {
			setWriteError();
			return 0;
		}
		if (k & 0x80) {						// it's a capital letter or other character reached with shift
			_keyReport.modifiers |= 0x02;	// the left shift modifier
			k &= 0x7F;
		}
	}

	// Add k to the key report only if it's not already present
	// and if there is an empty slot.
	if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
		_keyReport.keys[2] != k && _keyReport.keys[3] != k &&
		_keyReport.keys[4] != k && _keyReport.keys[5] != k) {

		for (i=0; i<6; i++) {
			if (_keyReport.keys[i] == 0x00) {
				_keyReport.keys[i] = k;
				break;
			}
		}
		if (i == 6) {
			setWriteError();
			return 0;
		}
	}
	sendReport(&_keyReport);
	return 1;
}

size_t BleDevice::press(const MediaKeyReport k)
{
    uint16_t k_16 = k[1] | (k[0] << 8);
    uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);

    mediaKeyReport_16 |= k_16;
    _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
    _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);

	sendReport(&_mediaKeyReport);
	return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t BleDevice::release(uint8_t k)
{
	uint8_t i;
	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	} else if (k >= 128) {	// it's a modifier key
		_keyReport.modifiers &= ~(1<<(k-128));
		k = 0;
	} else {				// it's a printing key
		k = pgm_read_byte(_asciimap + k);
		if (!k) {
			return 0;
		}
		if (k & 0x80) {							// it's a capital letter or other character reached with shift
			_keyReport.modifiers &= ~(0x02);	// the left shift modifier
			k &= 0x7F;
		}
	}

	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i=0; i<6; i++) {
		if (0 != k && _keyReport.keys[i] == k) {
			_keyReport.keys[i] = 0x00;
		}
	}

	sendReport(&_keyReport);
	return 1;
}

size_t BleDevice::release(const MediaKeyReport k)
{
    uint16_t k_16 = k[1] | (k[0] << 8);
    uint16_t mediaKeyReport_16 = _mediaKeyReport[1] | (_mediaKeyReport[0] << 8);
    mediaKeyReport_16 &= ~k_16;
    _mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
    _mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);

	sendReport(&_mediaKeyReport);
	return 1;
}

void BleDevice::releaseAll(void)
{
	_keyReport.keys[0] = 0;
	_keyReport.keys[1] = 0;
	_keyReport.keys[2] = 0;
	_keyReport.keys[3] = 0;
	_keyReport.keys[4] = 0;
	_keyReport.keys[5] = 0;
	_keyReport.modifiers = 0;
    _mediaKeyReport[0] = 0;
    _mediaKeyReport[1] = 0;
	sendReport(&_keyReport);
}

bool BleDevice::getLedStatus(uint8_t led)
{
	return (_keyboardLedsStatus & led) != 0;    
}

size_t BleDevice::write(uint8_t c)
{
	uint8_t p = press(c);  // Keydown
	release(c);            // Keyup
	return p;              // just return the result of press() since release() almost always returns 1
}

size_t BleDevice::write(const MediaKeyReport c)
{
	uint16_t p = press(c);  // Keydown
	release(c);            // Keyup
	return p;              // just return the result of press() since release() almost always returns 1
}

size_t BleDevice::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	while (size--) {
		if (*buffer != '\r') {
			if (write(*buffer)) {
			  n++;
			} else {
			  break;
			}
		}
		buffer++;
	}
	return n;
}

void BleDevice::click(uint8_t b)
{
  _buttons = b;
  move(0,0,0,0);
  _buttons = 0;
  move(0,0,0,0);
}

void BleDevice::move(signed char x, signed char y, signed char wheel, signed char hWheel)
{
  if ( connected )
  {
    uint8_t m[5];
    m[0] = _buttons;
    m[1] = x;
    m[2] = y;
    m[3] = wheel;
    m[4] = hWheel;
    this->inputMouse->setValue((uint8_t*)m, sizeof(m));
    this->inputMouse->notify();
    this->delay_ms(_delay_ms);
  }
}

void BleDevice::buttons(uint8_t b)
{
  if (b != _buttons)
  {
    _buttons = b;
    move(0,0,0,0);
  }
}

void BleDevice::mousePress(uint8_t b)
{
  buttons(_buttons | b);
}

void BleDevice::mouseRelease(uint8_t b)
{
  buttons(_buttons & ~b);
}

bool BleDevice::isPressed(uint8_t b)
{
  if ((b & _buttons) > 0)
    return true;
  return false;
}

void BleDevice::onStarted(BLEServer* pServer) {
}

void BleDevice::onConnect(BLEServer* pServer) {
  	// this->connected = pServer->getConnectedCount();
	this->connected = true;
	// pServer->getAdvertising()->start(); //do it :D

	// BLEDevice::getAddress();
	// BLEDevice::getDisconnectedClient();
	
	// pServer->disconnect(); // need test this

	// auto peer = pServer->getPeerInfo((this->connected)-1);

	// getConnHandle

	// }

	// ESP_LOGD(LOG_TAG, "Connected to: %d",peer.getAddress());
	// ESP_LOGD(LOG_TAG, "latency %d ms",peer.getConnLatency());
	// if(this->connected == 0){
		// BLEDevice::addIgnored(peer.getAddress());
	// ESP_LOGD(LOG_TAG, "Subscribed to Mouse: %d",inputMouse->getSubscribedCount());
		// inputMouse->setSubscribe(); //add or remove machine that connected 
	// }
	
	// delay_ms(10000);
	// pServer->disconnect(peer.getConnHandle());//CHECK IF NOT WHAT WE NEED ?
}
void BleDevice::onDisconnect(BLEServer* pServer) {
//   this->connected = false;
//   this->connected = pServer->getConnectedCount();
	this->connected = false;

	// reconnect ?


}

void BleDevice::onWrite(BLECharacteristic* me) {
  uint8_t* value = (uint8_t*)(me->getValue().c_str());
  (void)value;
  _keyboardLedsStatus = *value;
  ESP_LOGI(LOG_TAG, "special keys: %d", *value);
}

void BleDevice::delay_ms(uint64_t ms) {
  uint64_t m = esp_timer_get_time();
  if(ms){
    uint64_t e = (m + (ms * 1000));
    if(m > e){ //overflow
        while(esp_timer_get_time() > e) { taskYIELD();}
    }
    while(esp_timer_get_time() < e) {taskYIELD();}
  }
}