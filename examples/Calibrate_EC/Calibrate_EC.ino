#include "AtlasNW.h"

AtlasNW condProbe("conductivity", "SoftSerial", 7, 8, 9600);

void setup() {
  // put your setup code here, to run once:
  condProbe.calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
}
