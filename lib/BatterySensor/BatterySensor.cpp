#include "BatterySensor.h"


//CONSTRUCTOR
BatterySensor::BatterySensor(int alalogReadPin)
{
	_analogReadPin = alalogReadPin;
	_minVoltage = 3.5;
	_maxVoltage = 3.8;
}

int BatterySensor::readPercentage(){
	
	float currentVoltage = (float)analogRead(_analogReadPin) * 5.0 /1023.0;
	int returnPercentage;

	if (currentVoltage >= _maxVoltage){
		returnPercentage = 100;
	}else if (currentVoltage <= _minVoltage){
		returnPercentage = 0;
	}else{
		returnPercentage = (int) (((currentVoltage - _minVoltage) / (_maxVoltage - _minVoltage) * 100.0) + 0.5);
	}

	return returnPercentage;
}


