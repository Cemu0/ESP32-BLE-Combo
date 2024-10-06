/**
 * This example turns the ESP32 into a Bluetooth LE keyboard and mouse
 * Change the MAC address to connect with multiple device
 */
#include <Arduino.h>
#include "BleDevice.h"

BleDevice bleDevice("Device Name","Device Corp");

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(500000);
  Serial.println("Starting BLE work!");
  uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};//ca:70:f2:a1:19:51
  // uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
  // esp_base_mac_addr_set(new_mac);
  bleDevice.begin();
  // uint8_t new_mac[8] = {0x02,0x02,0x03,0x04,0x05,0x06};
  // uint8_t new_mac[8] = {0x03,0x02,0x03,0x04,0x05,0x06};
}

void loop() {
  digitalWrite(2,bleDevice.getLedStatus(LED_CAPS_LOCK));
  if(bleDevice.isConnected()) {
      if(millis() > 60000 && millis() < 80000 ){
        uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
        // uint8_t new_mac[8] = {0x00,0xcb,0xe7,0x35,0xc5,0x3f};
        Serial.println(esp_get_free_heap_size()); //198880

        bleDevice.end(); //bug if set true
        Serial.println("changeMac");
        Serial.println(esp_get_free_heap_size());
        esp_base_mac_addr_set(new_mac); 
        bleDevice.begin();
        Serial.println(esp_get_free_heap_size());

      } 
    
    bleDevice.print("Hello world");
    // bleDevice.move(20,20);
      unsigned long startTime;
    Serial.println("Scroll up");
    startTime = millis();
    while(millis()<startTime+4000) {
      // Serial.println("up");
      bleDevice.move(0,0,-1);
      delay(100);
    }

    bleDevice.mousePress(MOUSE_LEFT);
    delay(500);
    bleDevice.mouseRelease(MOUSE_LEFT);

  }

  Serial.println("Waiting 10 seconds...");
  delay(10000);


}