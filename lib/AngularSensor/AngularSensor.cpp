#include "AngularSensor.h"

//Contructor
AngularSensor::AngularSensor(int readAnalogPin){
	_readAnalogPin = readAnalogPin;
	_degScale = 360.0/1023.0;
}

//Public
double AngularSensor::readDeg(){
	return (double)analogRead(_readAnalogPin)*_degScale;
}

double AngularSensor::readBit(){
	return (double)analogRead(_readAnalogPin);
}