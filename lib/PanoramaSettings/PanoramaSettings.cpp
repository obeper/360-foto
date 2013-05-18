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
	_overlap = 0.2;
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
	_shutterSpeeds[32] = 101;
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
void PanoramaSettings::setPlusMinusEv(int ev){
	_plusMinusEv = ev;
}

int	PanoramaSettings::getMiddShutterSpeed(){
	return	_midShutterSpeed;
}

void PanoramaSettings::setMiddShutterSpeed(int code){
	int midShutterSpeedIndex = this->getShutterSpeedIndex(code);

	if(midShutterSpeedIndex != -1 ){
		_midShutterSpeed = code;
		_midShutterSpeedIndex = midShutterSpeedIndex;
	}
	
	
}

float PanoramaSettings::calcPanCordinate(int picNr){
	float horizontal_step = 360.0/_horizontalPics;
	if(picNr >= _totalNrOfPictures - 2.0){
		return 0.0;
	}else {
		return ((picNr % _horizontalPics)*horizontal_step);
	}	
} 
float PanoramaSettings::calcTiltCordinate(int picNr){
	if(picNr == _totalNrOfPictures - 2.0){
		return 90.0;
	}else if(picNr == _totalNrOfPictures - 1.0){
		return -90.0;
	}
	float tilt_start = 0.0 - (_verticalAngle - _fovVertical)/2;
	float tilt_stop = 180.0 - abs(tilt_start);
	float tilt_middle = tilt_stop - abs(tilt_start);
	float tilt_spann = abs(tilt_middle);
	float vertical_step = tilt_spann/_verticalPics;

	float tilt_row = ((picNr- (picNr % _horizontalPics))/_horizontalPics);
	return  (tilt_row*vertical_step) + tilt_start;
}

int PanoramaSettings::getShutterSpeed(int evNr){
	int newIndex = _midShutterSpeedIndex + evNr;
	newIndex = (newIndex > 54) ? 54 : newIndex;
	newIndex = (newIndex < 0) ? 0 : newIndex;
	return _shutterSpeeds[newIndex];
}



void PanoramaSettings::calcNrOfPictures(){

		/*
	_focalLength = 15.0; 
	_sensorHorizontal =22.3; 
	_sensorVertical =14.9; 
	_overlap = 1.2;
	*/

	_fovHorizontal = (2.0 * atan(_sensorHorizontal / (2.0 * _focalLength))) * (180.0/3.14);
	//73.3;

	_fovVertical = (2.0 * atan(_sensorVertical / (2.0 * _focalLength))) * (180.0/3.14);
	//52.85

	_horizontalPics = (int)((360.0/_fovHorizontal)*(1.0+_overlap) + 0.5);
	//6
	_verticalAngle = 180.0 - (1.0 - _overlap)*min(_fovHorizontal,_fovVertical);
	//137.72
	_verticalPics = (int)((_verticalAngle/_fovVertical)*(_overlap+1.0) + 0.5);
	//3
	_totalNrOfPictures = 2 + _horizontalPics*_verticalPics;
	

}
int PanoramaSettings::getShutterSpeedIndex(int code){
	for (int i = 0; i < 55; i++)
	{
		if(_shutterSpeeds[i] == code){
			return i;
		}
	}
	return -1;
}
