#include "PanoramaSettings.h"
#include <math.h>
//CONSTRUCTOR
PanoramaSettings::PanoramaSettings(float focal, float sensorH, float sensorV, float overlap)
{
	_focalLength = focal;
	_sensorHorizontal = sensorH;
	_sensorVertical = sensorV;
	_overlap = overlap;

	_plusMinusEv = 3;
	_midShutterSpeed = 356;
	_midShutterSpeedIndex = this->getShutterSpeedIndex(_midShutterSpeed);
	/*
	_focalLength = 15.0; 
	_sensorHorizontal =22.3; 
	_sensorVertical =14.9; 
	_overlap = 1.2;
	*/
	this->calcNrOfPictures();

	_shutterSpeeds[0] = 273;
	_shutterSpeeds[1] = 275;
	_shutterSpeeds[2] = 277;
	_shutterSpeeds[3] = 281;
	_shutterSpeeds[4] = 283;
	_shutterSpeeds[5] = 285;
	_shutterSpeeds[6] = 289;
	_shutterSpeeds[7] = 291;
	_shutterSpeeds[8] = 293;
	_shutterSpeeds[9] = 297;
	_shutterSpeeds[10] = 299;
	_shutterSpeeds[11] = 301;
	_shutterSpeeds[12] = 304;
	_shutterSpeeds[13] = 306;
	_shutterSpeeds[14] = 308;
	_shutterSpeeds[15] = 312;
	_shutterSpeeds[16] = 315;
	_shutterSpeeds[17] = 317;
	_shutterSpeeds[18] = 321;
	_shutterSpeeds[19] = 323;
	_shutterSpeeds[20] = 325;
	_shutterSpeeds[21] = 329;
	_shutterSpeeds[22] = 331;
	_shutterSpeeds[23] = 332;
	_shutterSpeeds[24] = 337;
	_shutterSpeeds[25] = 339;
	_shutterSpeeds[26] = 340;
	_shutterSpeeds[27] = 344;
	_shutterSpeeds[28] = 346;
	_shutterSpeeds[29] = 349;
	_shutterSpeeds[30] = 351;
	_shutterSpeeds[31] = 354;
	_shutterSpeeds[32] = 356;
	_shutterSpeeds[33] = 359;
	_shutterSpeeds[34] = 362;
	_shutterSpeeds[35] = 364;
	_shutterSpeeds[36] = 367;
	_shutterSpeeds[37] = 370;
	_shutterSpeeds[38] = 372;
	_shutterSpeeds[39] = 375;
	_shutterSpeeds[40] = 378;
	_shutterSpeeds[41] = 380;
	_shutterSpeeds[42] = 383;
	_shutterSpeeds[43] = 386;
	_shutterSpeeds[44] = 388;
	_shutterSpeeds[45] = 391;
	_shutterSpeeds[46] = 394;
	_shutterSpeeds[47] = 396;
	_shutterSpeeds[48] = 399;
	_shutterSpeeds[49] = 402;
	_shutterSpeeds[50] = 404;
	_shutterSpeeds[51] = 407;
	_shutterSpeeds[52] = 410;
	_shutterSpeeds[53] = 412;
	_shutterSpeeds[54] = 415;
}
int PanoramaSettings::getNrOfPictures(){	
	return _totalNrOfPictures;
}
void PanoramaSettings::setFocalLength(float focal){
	_focalLength = focal;
	this->calcNrOfPictures();
}

int PanoramaSettings::getPlusMinusEv(){	
	return _plusMinusEv;
}
void PanoramaSettings::setplusMinusEv(int ev){
	_plusMinusEv = ev;
}

int	PanoramaSettings::getMiddShutterSpeed(){
	return	_midShutterSpeed;
}

void PanoramaSettings::setMiddShutterSpeed(int code){
	_midShutterSpeedIndex = this->getShutterSpeedIndex(code);
	_midShutterSpeed = code;
}

float PanoramaSettings::calcPanCordinate(int picNr){
	if(picNr == 0) return 0;
	return picNr*(_verticalAngle/_verticalPics);
	//FEEEEL
} 
float PanoramaSettings::calcTiltCordinate(int picNr){
	if(picNr == 0) return 0;
	return picNr*(360/_horizontalPics);
	//FEEEL
}

int PanoramaSettings::getShutterSpeed(int evNr){
	int newIndex = _midShutterSpeedIndex + evNr;
	return newIndex;//_shutterSpeeds[newIndex];
}
void PanoramaSettings::calcNrOfPictures(){
	float fovHorizontal = (2.0 * atan(_sensorHorizontal / (2.0 * _focalLength))) * (180.0/3.14);
	float fovVertical =   (2.0 * atan(_sensorVertical / (2.0 * _focalLength))) * (180.0/3.14);

	_horizontalPics = (int)(((360/fovHorizontal)*_overlap)+0.5);
	_verticalAngle = 180 - min(fovHorizontal,fovVertical);
	_verticalPics = (int)((_verticalAngle/fovVertical)*_overlap + 0.5);

	_totalNrOfPictures = 2.0 + _horizontalPics*_verticalPics;
	

}
int PanoramaSettings::getShutterSpeedIndex(int code){
	for (int i = 0; i < 55; i++)
	{
		if(_shutterSpeeds[i] == code){
			return i;
		}
	}
	return 27;
	return 0;
}
