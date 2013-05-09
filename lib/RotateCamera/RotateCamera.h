#ifndef RotateCamera_h
#define RotateCamera_h

#include "Arduino.h"
#include "StepperControl.h"
#include "AngularSensor.h"

class RotateCamera
{
	public:
		//CONSTRUCOR
		RotateCamera(int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
					 int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin): 
			_sensorTilt(tiltReadAnalogPin), 
			_sensorPan(panReadAnalogPin),
			_stepperTilt(tiltStepperDirPin,tiltStepperStepPin),
			_stepperPan(panStepperStepPin, panStepperStepPin){
				//BASE SPEED FOR STEPPERMOTORS
				_speed = 0.1;
				//GEAR RATIO BETWEEN MOTOR AND CAMERA MOVEMENT
				_gearRatio = 50;
			}
		
		void move(float panCordinate, float tiltCordinate);
	private:
		AngularSensor 		_sensorTilt;
		AngularSensor 		_sensorPan;

		StepperControl 		_stepperTilt;
		StepperControl 		_stepperPan;

		float 				_speed;
		int					_gearRatio;

		void pan(float cordinate);
		void tilt(float cordinate);
};

#endif
