/**
 * This example check the BLE keyboard leds
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
      Serial.print("Scroll Lock on ");
    }
      if (bleCombo.getLedStatus(LED_NUM_LOCK))
    {
      Serial.print("Num Lock on ");
    }
    // bleKeyboard.write('a');
    bleCombo.print("Hello");
  }

  // Serial.println("Waiting 5 seconds...");
  delay(10000);
}