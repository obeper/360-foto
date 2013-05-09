#ifndef StepperControl_h
#define StepperControl_h

#include "Arduino.h"

class StepperControl
{
	public:
		StepperControl(int dirPin, int stepPin);
		void run(float deg, float speed);
	private:
		int 		_dirPin;
		int			_stepPin;
		float		_stepperResolution;
};

#endif
