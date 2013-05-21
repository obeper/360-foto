#include "RotateCamera.h"

//Public
void RotateCamera::move(float panCordinate, float tiltCordinate){

	//this->tilt(tiltCordinate);
	this->pan(panCordinate);
	_tiltInPosition = true;
	//_stepperTilt.runSpeedToPosition();
	_stepperPan.runSpeedToPosition();


}
bool RotateCamera::inPosition(){
	return (_tiltInPosition && _panInPosition);
}
//Private
void RotateCamera::pan(float cordinate){
	//Calc degs to move
	static long lastPanSensorReadTime = 0;
	long currentTime = millis();

	if(currentTime > lastPanSensorReadTime + 1){
		_currentPanCordinate = (float)_sensorPan.readDeg();
		lastPanSensorReadTime = currentTime;
	}
	float degsToMove = this->pathToMove(cordinate, _currentPanCordinate);
	

	_panInPosition = (abs(cordinate - _currentPanCordinate) < _satAngle) ? true : false;
	//Move camera
	if(!_panInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperPan.move(steppsToMove);
		_stepperPan.setSpeed(_speed);
	}
	
}

void RotateCamera::tilt(float cordinate){
	//Calc degs to move

	static long lastTiltSensorReadTime = 0;
	long currentTime = millis();

	if(currentTime > lastTiltSensorReadTime + 1){
		_currentPanCordinate = (float)_sensorPan.readDeg();
		lastTiltSensorReadTime = currentTime;
	}
	float degsToMove = this->pathToMove(cordinate, _currentPanCordinate);
	

	_tiltInPosition = (abs(cordinate - _currentPanCordinate) < _satAngle) ? true : false;
	//Move camera
	if(!_tiltInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperPan.move(steppsToMove);
		_stepperPan.setSpeed(_speed);
	}
	
}

float RotateCamera::calcSteppsToMove(float degs){
	return 1600.0/360.0 * degs;
}

float RotateCamera::pathToMove(float cordinate, float currentCordinate){
	//SHORTEST PATH TO CORD
	float degsToMove;
	float dir = ((cordinate - currentCordinate) < 0) ? 1.0 : -1.0;
	float deg11 = cordinate - currentCordinate;
	float deg1 = abs(deg11);
	float deg2 = 360.0 -  deg1;
	if(deg1 < deg2){
		degsToMove = deg1*(-1.0)*dir;
	}else{
		degsToMove = deg2*dir;
	}
	return	degsToMove;
}