# ESP32 BLE Combo library
This builds on the BLE-Combo library, and allows you to toggle keyboard, mouse or both. It will enable whichever you select.
This is built on the NimBLE library.

Build on the tweaks by ESP32-BLE-Combo.

# Usage
That was built on the hard work of:
- [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)

You must [activate NimBLE](https://github.com/T-vK/ESP32-BLE-Keyboard#how-to-activate-nimble-mode) for this library to work

# Small features
This library support check keyboard status led.   
If you need to switch between devices, check the 'testComboMultiple' example.    
On ESP32-C3, there's no bluetooth, so this won't work.

# Example 
Check the examples folder for more examples.

``` C++
/**
 * This example turns the ESP32 into a Bluetooth LE keyboard and mouse
 */
#include <Arduino.h>
#include <BleDevice.h>

BleDevice bleDevice("Device Name","Device Corp");

void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  bleDevice.begin();
  Serial.println("Starting BLE work!");
}

void loop() {
  if(bleDevice.isConnected()) {
    
    bleDevice.print("Hello world");
    unsigned long startTime;

    Serial.println("Scroll up");
    startTime = millis();

    while(millis()<startTime+4000) {
      Serial.println("up");
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
```
# Tested on
- windows 10  
- ios 16: note if you can't press the space button check [here](https://developer.apple.com/forums/thread/119022)
- MacOS 12.6
- IpadOS 15

# Working not correct on
- Android (6, 10) just detect keyboard/mouse if one of them shown up first in the begin function 

