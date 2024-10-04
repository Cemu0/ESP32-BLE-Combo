/**
 * This example turns the ESP32 into a Bluetooth LE keyboard and mouse
 */
#include <Arduino.h>
#include "BleDevice.h"

BleDevice bleDevice("Device Name","Device Corp");

void setup() {
  Serial.begin(115200);
  bleDevice.begin(true,true);

  Serial.println("Starting BLE work!");
}

void loop() {
  if(bleDevice.isConnected()) {
    
    bleDevice.write(KEY_RETURN);
    // bleDevice.write(KEY_MEDIA_PLAY_PAUSE);
    bleDevice.print("Hello world");
    
    delay(10000);
  } else {
    printf( "-\n");
    delay(2000);
  }
}
