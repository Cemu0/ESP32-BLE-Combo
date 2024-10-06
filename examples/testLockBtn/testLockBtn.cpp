/**
 * This example check the BLE keyboard leds
 */
#include <Arduino.h>
#include "BleDevice.h"

BleDevice bleDevice("Device Name","Device Corp");

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // just keyboard
  bleDevice.begin(false,true);
}

void loop() {
  if(bleDevice.isConnected()) {
    digitalWrite(2,bleDevice.getLedStatus(LED_CAPS_LOCK));
    if (bleDevice.getLedStatus(LED_CAPS_LOCK))
    {
      Serial.print("Cap Lock on ");
    }
    if (bleDevice.getLedStatus(LED_SCROLL_LOCK))
    {
      Serial.print("Scroll Lock on ");
    }
      if (bleDevice.getLedStatus(LED_NUM_LOCK))
    {
      Serial.print("Num Lock on ");
    }
    // bleKeyboard.write('a');
    bleDevice.print("Hello");
    delay(10000);
    
  } else {
    printf( "-\n");
    delay(2000);
  }
  
}