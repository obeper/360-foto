#include "BatterySensor.h"

//enum displayState { BLANK, ON, READY, RECEIVINGDATA, START, RUNNING, PAUSE, RESUME, RESET, DONE  };


//CONSTRUCTOR
BatterySensor::BatterySensor(int alalogReadPin)
{
	_analogReadPin = alalogReadPin;
	_minVoltage = 3.5;
	_maxVoltage = 3.8;
}

