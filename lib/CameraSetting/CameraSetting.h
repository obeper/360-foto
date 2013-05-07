
#ifndef CameraSetting_h
#define CameraSetting_h

#include "Arduino.h"
#include "MemoryStorage.h"

struct configScheme
{
    int numberOfExposures = 7; // st
    int EVRange = 6; // 
    int focalDistance = 35; // mm
    int minShutterTime =  125 //micoSek
};

enum CameraSettingProperty { numberOfExposures, EVRange, focalDistance, minShutterTime};

class CameraSetting
{
public:
	CameraSetting(int serialProt);
	readProperty(String propertyName);
	listenForPropertyChange();
private:

	configScheme 	_config;
	int 			_serialProt;


	updateProperty(String propertyName, int propertyValue);
};

#endif