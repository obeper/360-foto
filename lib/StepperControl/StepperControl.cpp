#include "StepperControl.h"


StepperControl::StepperControl(int dirPin, int stepPin){
	_dirPin = dirPin;
	_stepPin = stepPin;

	//YOUR ENGINE RESOLUTION
	_stepperResolution = 0.225;

	pinMode(_dirPin, OUTPUT);
	pinMode(_stepPin, OUTPUT);
}

void StepperControl::run(float deg, float speed ){
	//Speed 0.01 - 1 1 = FASTEST
	int dir = (deg > 0) ? HIGH:LOW;
	digitalWrite(_dirPin, dir);

	int steps = abs(deg) * (1/_stepperResolution);
	float usDelay = (1/speed) * 70;

	for (int i = 0; i < steps; i++)
	{
		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(usDelay);
		digitalWrite(_stepPin, LOW);
		delayMicroseconds(usDelay);
	}
}