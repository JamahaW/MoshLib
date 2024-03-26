#include <Arduino.h>
#include <MoshLib.hpp>


void setup() {
  delay(1000);

  setDistF(us);
  goWallFront(30);

  goDist(-500);
  goWallFront(ir1, 30, 20);

  goDist(-500);
}

void loop() {}
