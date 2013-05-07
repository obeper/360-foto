#include "AngularSensor.h"

//Contructor
void AngularSensor::AngularSensor(int readAnalogPin){
	_readAnalogPin = readAnalogPin;
	_degSacle = 360.0/1015.0;
}

//Public
double AngularSensor::readDeg(){
	return (double)analogRead(analogPin)*_degSacle;
}