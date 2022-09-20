/**
 * This example turns the ESP32 into a Bluetooth LE keyboard and mouse
 */
#include <Arduino.h>
#include <BleCombo.h>

BleCombo bleCombo;

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  bleCombo.begin();
  Serial.println("Starting BLE work!");
}

void loop() {
  if(bleCombo.isConnected()) {
    
    bleCombo.print("Hello world");
    unsigned long startTime;

    Serial.println("Scroll up");
    startTime = millis();

    while(millis()<startTime+4000) {
      Serial.println("up");
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