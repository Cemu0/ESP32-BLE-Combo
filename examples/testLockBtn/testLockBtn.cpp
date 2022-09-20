/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    digitalWrite(2,bleKeyboard.getLedStatus(LED_CAPS_LOCK));
    if (bleKeyboard.getLedStatus(LED_CAPS_LOCK))
    {
      Serial.print("Cap Lock on ");
    }
    if (bleKeyboard.getLedStatus(LED_SCROLL_LOCK))
    {
      Serial.print("Scroll Lock on "); //this not work ?
    }
      if (bleKeyboard.getLedStatus(LED_NUM_LOCK))
    {
      Serial.print("Num Lock on ");
    }
    // bleKeyboard.write('a');
    bleKeyboard.print("Hello");
  }

  // Serial.println("Waiting 5 seconds...");
  delay(10000);
}