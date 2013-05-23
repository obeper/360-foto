#ifndef AngularSensor_h
#define AngularSensor_h

#include "Arduino.h"

class AngularSensor
{
public:
	AngularSensor(int readAnalogPin);
	double readDeg();
	double readBit();
	void calibrate(double angle);

private:

	int 	_readAnalogPin;
	double 	_degScale;
	double	_calibrate;
};

#endif