#include "AngularSensor.h"

//Contructor
AngularSensor::AngularSensor(int readAnalogPin){
	_readAnalogPin = readAnalogPin;
	_degScale = 360.0/1023.0;
	_calibrate = 0.0;
}

//Public
double AngularSensor::readDeg(){
	
	double deg = (double)analogRead(_readAnalogPin)*_degScale;// + _calibrate;
	/*
	if(deg < 0){
		deg += 360;
	}
	*/
	return deg;

}

double AngularSensor::readBit(){
	return (double)analogRead(_readAnalogPin);
}
void AngularSensor::calibrate(double angle){
	_calibrate = angle;
}