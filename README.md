# ESP32 BLE Combo library
Using Keyboard and Mouse at the same time 
This use the NimBLE library as default 
Also worked on ESP32-C3 

# Usage

You must [activate NimBLE](https://github.com/T-vK/ESP32-BLE-Keyboard#how-to-activate-nimble-mode) for this library to work

This library have all the feature of the original library. Please check them for usage details:
- [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)  

You might also be interested in:
<!-- TODO: add this library into the combo library -->
- [ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)

# Small features
This library support check keyboard status led.  
If you need to switch between devices, check the 'testComboMultiple' example.  

# Example 
Check the examples folder for more examples.

```
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
```
# Tested on
- windows 10  
- ios 16: note if you can't press the space button check [here](https://developer.apple.com/forums/thread/119022)
- MacOS 12.6

# Working not correct on
- Android (6, 10) just detect keyboard/mouse if one of them shown up first in the begin function 

