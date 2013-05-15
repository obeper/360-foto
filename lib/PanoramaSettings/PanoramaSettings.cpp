#include "PanoramaSettings.h"
#include <math.h>
//CONSTRUCTOR
PanoramaSettings::PanoramaSettings(float focal, float sensorH, float sensorV, float overlap)
{
	_focalLength = focal;
	_sensorHorizontal = sensorH;
	_sensorVertical = sensorV;
	_overlap = overlap;
	/*
	_focalLength = 15.0; 
	_sensorHorizontal =22.3; 
	_sensorVertical =14.9; 
	_overlap = 1.2;
	*/
	this->calcNrOfPictures();
}

int PanoramaSettings::getNrOfPictures(){
	return _totalNrOfPictures;
}
void PanoramaSettings::setFocalLength(float focal){
	_focalLength = focal;
	this->calcNrOfPictures();
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


void PanoramaSettings::calcNrOfPictures(){
	float fovHorizontal = (2.0 * atan(_sensorHorizontal / (2.0 * _focalLength))) * (180.0/3.14);
	float fovVertical =   (2.0 * atan(_sensorVertical / (2.0 * _focalLength))) * (180.0/3.14);

	_horizontalPics = (int)(((360/fovHorizontal)*_overlap)+0.5);
	_verticalAngle = 180 - min(fovHorizontal,fovVertical);
	_verticalPics = (int)((_verticalAngle/fovVertical)*_overlap + 0.5);

	_totalNrOfPictures = 2.0 + _horizontalPics*_verticalPics;
	

}
