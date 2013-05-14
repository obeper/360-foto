#ifndef RotateCamera_h
#define RotateCamera_h

#include "Arduino.h"
#include "AccelStepper.h"
#include "AngularSensor.h"

class RotateCamera
{
	public:
		//CONSTRUCOR
		RotateCamera(int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
					 int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin): 
			_sensorTilt(tiltReadAnalogPin), 
			_sensorPan(panReadAnalogPin),
			_stepperTilt(1,tiltStepperStepPin,tiltStepperDirPin),
			_stepperPan(1,panStepperStepPin, panStepperDirPin){
				//BASE SPEED FOR STEPPERMOTORS
				_speed = 5q000;
				//GEAR RATIO BETWEEN MOTOR AND CAMERA MOVEMENT
				_gearRatio = 50;

				_stepperTilt.setMaxSpeed(6000);
				_stepperPan.setMaxSpeed(6000);
				
			}
		

		
		void move(float panCordinate, float tiltCordinate);
	private:
		AngularSensor 		_sensorTilt;
		AngularSensor 		_sensorPan;

		AccelStepper 		_stepperTilt;
		AccelStepper 		_stepperPan;

		float 				_speed;
		int					_gearRatio;

		void pan(float cordinate);
		void tilt(float cordinate);
		float calcSteppsToMove(float degs);
};

#endif
