#include "RotateCamera.h"

//Public
void RotateCamera::move(float panCordinate, float tiltCordinate){

	this->tilt(tiltCordinate);
	this->pan(panCordinate);
	_stepperTilt.runSpeedToPosition();
	_stepperPan.runSpeedToPosition();


}
bool RotateCamera::inPosition(){
	return (_tiltInPosition && _panInPosition);
}
//Private
void RotateCamera::pan(float cordinate){
	//Calc degs to move
	float currentCordinate = (float)_sensorPan.readDeg();
	float degsToMove = cordinate - currentCordinate;
	_panInPosition = (degsToMove < _satAngle) ? true : false;
	//Move camera
	if(!_panInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperPan.move(steppsToMove);
		_stepperPan.setSpeed(_speed);
	}
}

void RotateCamera::tilt(float cordinate){
	//Calc degs to move
	float currentCordinate = (float)_sensorTilt.readDeg();
	float degsToMove = cordinate - currentCordinate;
	_tiltInPosition = (degsToMove < _satAngle) ? true : false;
	//Move camera
	if(!_tiltInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperTilt.move(steppsToMove);
		_stepperTilt.setSpeed(_speed);
	}
}

float RotateCamera::calcSteppsToMove(float degs){
	return 1600.0/360.0 * degs;
}