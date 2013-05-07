#include "BatterySensor.h"


BatterySensor bat1(2);

void setup() {
  
}

void loop() {

	Serial.print(bat1.readPercentage());
  
}