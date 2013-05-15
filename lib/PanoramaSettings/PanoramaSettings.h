#ifndef PanoramaSettings_h
#define PanoramaSettings_h


#include "Arduino.h"


class PanoramaSettings
{
  public:

  	//CONSTRUCT
    PanoramaSettings(float focal, float sensorH, float sensorV, float overlap);

    
  private:
  	float _focalLength; 
	float _sensorHorizontal; 
	float _sensorVertical; 
	float _overlap;
	void calc();
   
};


#endif