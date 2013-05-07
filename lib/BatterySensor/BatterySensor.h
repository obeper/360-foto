
#ifndef BatterySensor_h
#define BatterySensor_h

#include "Arduino.h"

class BatterySensor
{
  public:
  	//CONSTRUCT
    BatterySensor(int analogReadPin);
  private:
   int      _analogReadPin;
   float    _minVoltage;
   float    _maxValtage;
   float    _currentVoltage;  
};


#endif