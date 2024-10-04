#include <Arduino.h>
#include "BleDevice.h"

BleDevice bleDevice("Device Name","Device Corp");

static int movement = 1;

void setup() {
  Serial.begin(115200);
  delay(3000);
  printf("Starting Mouse Only\n");
  // characteristic 'mouse'
  bleDevice.begin(true);


}

void loop() {
  if(bleDevice.isConnected()) {
    printf("moving mouse %d dot(s) (%ld) \n", movement);
    
    // bleDevice.mousePress(MOUSE_LEFT);
    // bleDevice.mouseRelease(MOUSE_LEFT);
    bleDevice.move(movement, 0);
    movement += 1;

    delay(5000);

  } else {
    printf( "-\n");
    delay(2000);
  }
}
