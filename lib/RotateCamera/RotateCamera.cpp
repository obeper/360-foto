#include "RotateCamera.h"

//Public
void RotateCamera::move(float panCordinate, float tiltCordinate){

	this->tilt(tiltCordinate);
	this->pan(panCordinate);



}
bool RotateCamera::inPosition(){
	return (_tiltInPosition && _panInPosition);
}

void RotateCamera::stop(){
	_stepperTilt.stop();
	_stepperPan.stop();
}
void RotateCamera::runUp(){
	float tiltDeg = _sensorTilt.readDeg();
	//if(tiltDeg > 270 && tiltDeg < 90){
		_stepperTilt.setSpeed(_speed); 
		_stepperTilt.runSpeed();
	//} 
}
void RotateCamera::runDown(){
	float tiltDeg = _sensorTilt.readDeg();
	//if(tiltDeg > 270 && tiltDeg < 90){
		_stepperTilt.setSpeed(-1*_speed); 
		_stepperTilt.runSpeed();
	//}
}
void RotateCamera::runLeft(){
	_stepperPan.setSpeed(-1*_speed); 
	_stepperPan.runSpeed(); 
}
void RotateCamera::runRight(){
	_stepperPan.setSpeed(_speed); 
	_stepperPan.runSpeed(); 
}

//Private
void RotateCamera::pan(float cordinate){
	//Calc degs to move

		_currentPanCordinate = (float)_sensorPan.readDeg();

	float degsToMove = this->pathToMove(cordinate, _currentPanCordinate);


	_panInPosition = (abs(cordinate - _currentPanCordinate) < _satAngle) ? true : false;
	//Move camera
	if(!_panInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperPan.move(steppsToMove);
		_stepperPan.setSpeed(_speed);
		_stepperPan.runSpeedToPosition();
	}
	
}

void RotateCamera::tilt(float cordinate){
	//Calc degs to move

	_currentTiltCordinate =  (float)_sensorTilt.readDeg();
	if(cordinate < 0.0 ){
		cordinate += 360.0;
	}
		
	/*
	if(cordinate > 90 || cordinate < 270){
		return;
	}
	*/
	float degsToMove = -1.0*this->pathToMove(cordinate, _currentTiltCordinate);


	_tiltInPosition = (abs(cordinate - _currentTiltCordinate) < _satAngle) ? true : false;
	//Move camera
	if(!_tiltInPosition){
		float steppsToMove = this->calcSteppsToMove(degsToMove) * _gearRatio;
		_stepperTilt.move(steppsToMove);
		_stepperTilt.setSpeed(_speed);
		_stepperTilt.runSpeedToPosition();
	}
	
}

float RotateCamera::calcSteppsToMove(float degs){
	return 1600.0/360.0 * degs;
}

float RotateCamera::pathToMove(float cordinate, float currentCordinate){
	//SHORTEST PATH TO CORD
	float degsToMove;
	float dir = ((cordinate - currentCordinate) < 0.0) ? 1.0 : -1.0;
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