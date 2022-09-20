/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <Arduino.h>
#include <BleCombo.h>

BleCombo bleCombo;

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleCombo.begin();
}

void loop() {
  if(bleCombo.isConnected()) {
    digitalWrite(2,bleCombo.getLedStatus(LED_CAPS_LOCK));
    if (bleCombo.getLedStatus(LED_CAPS_LOCK))
    {
      Serial.print("Cap Lock on ");
    }
    if (bleCombo.getLedStatus(LED_SCROLL_LOCK))
    {
      Serial.print("Scroll Lock on "); //this not work ?
    }
      if (bleCombo.getLedStatus(LED_NUM_LOCK))
    {
      Serial.print("Num Lock on ");
    }
    // bleKeyboard.write('a');
    bleCombo.print("Hello world");
    // bleCombo.move(20,20);
        unsigned long startTime;

    Serial.println("Scroll up");
    startTime = millis();
    while(millis()<startTime+4000) {
      // Serial.println("up");
      bleCombo.move(5,0,0);
      delay(100);
    }
    bleCombo.mousePress(MOUSE_LEFT);
    delay(500);
    bleCombo.mouseRelease(MOUSE_LEFT);

  }

  // Serial.println("Waiting 5 seconds...");
  delay(10000);
}