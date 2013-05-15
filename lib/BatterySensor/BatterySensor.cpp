#include "BatterySensor.h"


//CONSTRUCTOR
BatterySensor::BatterySensor(int analogReadPin)
{
	_analogReadPin = analogReadPin;
	_minVoltage = 3.3;
	_maxVoltage = 3.8;
	for (int i = 0; i < 12; i++)
	{
		_voltageList[i] = 0.0;
	}
	
	_arrayPosition = 0;
}

int BatterySensor::readPercentage(){
	
	float currentVoltage = _meanVoltage;
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

void BatterySensor::readVoltage(){
	float currentVoltage = (float)analogRead(_analogReadPin) * 5.0 /1023.0;
	_voltageList[_arrayPosition] = currentVoltage;
	_meanVoltage = this->mean();
	if(_arrayPosition <= 10){
		_arrayPosition++;
	}else{
		_arrayPosition = 0;
	}
	
}

float BatterySensor::mean(){
	float sum = 0;
	for (int i = 0; i <= _arrayPosition; i++)
	{
		sum += _voltageList[i];
	}
	return sum/(_arrayPosition+1);
}