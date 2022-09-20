/**
 * This example turns the ESP32 into a Bluetooth LE keyboard and mouse
 * Change the MAC address to connect with multiple device
 */
#include <Arduino.h>
#include <BleCombo.h>

BleCombo bleCombo;

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(500000);
  Serial.println("Starting BLE work!");
  uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};//ca:70:f2:a1:19:51
  // uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
  // esp_base_mac_addr_set(new_mac);
  bleCombo.begin();
  // uint8_t new_mac[8] = {0x02,0x02,0x03,0x04,0x05,0x06};
  // uint8_t new_mac[8] = {0x03,0x02,0x03,0x04,0x05,0x06};
}

void loop() {
  digitalWrite(2,bleCombo.getLedStatus(LED_CAPS_LOCK));
  if(bleCombo.isConnected()) {
      if(millis() > 60000 && millis() < 80000 ){
        uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
        // uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
        Serial.println(esp_get_free_heap_size()); //198880

        bleCombo.end(); //bug if set true
        Serial.println("changeMac");
        Serial.println(esp_get_free_heap_size());
        esp_base_mac_addr_set(new_mac); 
        bleCombo.begin();
        Serial.println(esp_get_free_heap_size());

      } 
    
    bleCombo.print("Hello world");
    // bleCombo.move(20,20);
      unsigned long startTime;
    Serial.println("Scroll up");
    startTime = millis();
    while(millis()<startTime+4000) {
      // Serial.println("up");
      bleCombo.move(0,0,-1);
      delay(100);
    }

    bleCombo.mousePress(MOUSE_LEFT);
    delay(500);
    bleCombo.mouseRelease(MOUSE_LEFT);

  }

  Serial.println("Waiting 10 seconds...");
  delay(10000);


}