#include "RotateCamera.h"

//Public
void RotateCamera::move(float panCordinate, float tiltCordinate){
	this->tilt(tiltCordinate);
	this->pan(panCordinate);
	
}

//Private
void RotateCamera::pan(float cordinate){
	//Calc degs to move
	float currentCordinate = (float)_sensorPan.readDeg();
	float degsToMove = cordinate - currentCordinate;
	//Move camera
	_stepperPan.run(degsToMove * _gearRatio, _speed);
}

void RotateCamera::tilt(float cordinate){
	//Calc degs to move
	float currentCordinate = (float)_sensorTilt.readDeg();
	float degsToMove = cordinate - currentCordinate;
	//Move camera
	_stepperTilt.run(degsToMove * _gearRatio, _speed);
}