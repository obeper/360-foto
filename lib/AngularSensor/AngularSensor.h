#ifndef AngularSensor_h
#define AngularSensor_h

#include "Arduino.h"

class AngularSensor
{
public:
	AngularSensor(int readAnalogPin);
	double readDeg();
	double readBit();

private:

	int 	_readAnalogPin;
	double 	_degScale;
};

#endif