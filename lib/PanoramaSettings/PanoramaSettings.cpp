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
}

void PanoramaSettings::calc(){
float fovHorizontal = (2.0 * atan(_sensorHorizontal / (2.0 * _focalLength))) * (180.0/3.14);
float fovVertical =   (2.0 * atan(_sensorVertical / (2.0 * _focalLength))) * (180.0/3.14);

int horizontalPics = (int)(((360/fovHorizontal)*_overlap)+0.5);
float verticalAngle = 180 - min(fovHorizontal,fovVertical);
int verticalPics = (int)((verticalAngle/fovVertical)*_overlap + 0.5);

int totalNrOfPics = 2.0 + horizontalPics*verticalPics;


}
