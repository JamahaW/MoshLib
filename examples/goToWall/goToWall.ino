#include <Arduino.h>
#include <MoshLib.hpp>


void setup() {
  delay(1000);

  distSensorF(us);
  goToWall(30);

  goDirect(-500);
  goToWall(ir1, 30, 20);

  goDirect(-500);
}

void loop() {}
