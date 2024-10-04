#include <Arduino.h>
#include "BleDevice.h"

std::string deviceName = "MouseKeyboard";
std::string deviceCorp = "Microsoft";
// instantiate class with settings
BleDevice bleDevice(deviceName, deviceCorp, ESP_PWR_LVL_P3); // @ 3dB transmit power

static int movement = 4;

void setup() {
  Serial.begin(115200);
  delay(3000);
  printf("Starting\n");

  bool enableMouse = true;
  bool enableKeyboard = true;
  // start device, enabling mouse and/or keyboard...
  bleDevice.begin(enableMouse,enableKeyboard);
  printf( "[%s]: enabled with %s%s", deviceName.c_str(), enableMouse ? "[Mouse]" : "", enableKeyboard ? " [Keyboard]" : "" );
}

void loop() {
  if(bleDevice.isConnected()) {
    
    // bleDevice.mousePress(MOUSE_LEFT);
    // bleDevice.mouseRelease(MOUSE_LEFT);
    // bleDevice.write(KEY_RETURN);
    // bleDevice.write(KEY_MEDIA_PLAY_PAUSE);
    // bleDevice.print("Hello world");
    bleDevice.print("~");
    bleDevice.write(KEY_CAPS_LOCK);
    bleDevice.move(movement, 0);
    movement = -movement;
    printf( " .. attempted typing ~ and moving mouse...\n");

    delay(5000);
    
  } else {
    printf( "-\n");
    delay(2000);
  }
}
