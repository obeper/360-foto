#include "CameraSetting.h"

//Contructor
void CameraSetting::CameraSetting(int serialPort){
	 MemoryStorage_read(0, _config);

	 _serialPort = serialPort;
	 switch(serialPort){
	 	case 0:
	 	Serial.begin(115200);
	 	break;
	 	case 1:
	 	Serial1.begin(115200);
	 	break;
	 	case 2:
	 	Serial2.begin(115200);
	 	break;
	 }
}

//Public
int CameraSetting::readProperty(CameraSettingProperty propertyName){
	int property;

	switch(propertyName){
		case numberOfExposures:
			property = _config.numberOfExposures;
		break;
		case EVRange:
			property = _config.EVRange;
		break;
		case focalDistance:
			property = _config.focalDistance;
		break;
		case minShutterTime:
			property = _config.minShutterTime;
		break;
	}

}

void CameraSetting::listenForPropertyChange(){

	char serialValue[10];
	int i = 0;

	switch(_serialPort){
	 	case 0:
		 	while(Serial.available()){

		 		serialValue[i] = Serial.read();
		 	}
	 		break;
	 	case 1:
		 	while(Serial1.available()){
		 		serialValue = Serial1.read();
		
		 	}
	 		break;
	 	case 2:
		 	while(Serial2.available()){
		 		serialValue = Serial2.read();
		 		
		 	}
	 		break;
	 }

	 int propertyName = serialValue[0];
	 int propertyValue = serialValue[2];
	 		//TODO READ THE OUTPUT AND SAVE TO CONFIG
	 		//updateProperty(propertyName, propertyValue);

}

bool CameraSetting::updateProperty(CameraSettingProperty propertyName, int propertyValue){

	switch(propertyName){
		case numberOfExposures:
			_config.numberOfExposures = propertyValue;
		break;
		case EVRange:
			_config.EVRange = propertyValue;
		break;
		case focalDistance:
			_config.focalDistance = propertyValue;
		break;
		case minShutterTime:
			_config.minShutterTime = propertyValue;
		break;
	}
	return this->saveConfig();
}

//Private

bool CameraSetting::saveConfig(){
	 if(MemoryStorage_write(0, _config) > 0) return true;
	 return false;
}