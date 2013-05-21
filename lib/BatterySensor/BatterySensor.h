
#ifndef BatterySensor_h
#define BatterySensor_h

#include "Arduino.h"

class BatterySensor
{
  public:
  	//CONSTRUCT
    BatterySensor(int analogReadPin);
    int readPercentage();
    void readVoltage();
  private:
   int    _analogReadPin;
   float  _minVoltage;
   float  _maxVoltage;
   float	_voltageList[12];
   int		_arrayPosition;
   float 	_meanVoltage;

   float mean();
};


#endif