#include "Arduino.h"
#include "RotateCamera.h"


//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin
RotateCamera camera(2, 3, 4, 6, 7, 8);

void setup() {
  
}

void loop() {

	camera.move(30,20);
	delay(5000);
  
}