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
				_speedTilt = 120;
				_speed = 5000;
				//GEAR RATIO BETWEEN MOTOR AND CAMERA MOVEMENT
				_gearRatio = 50;

				_satAngle = 2.0;
				_sensorTilt.calibrate(-245.0);
				_stepperTilt.setMaxSpeed(1000);
				_stepperPan.setMaxSpeed(6000);
				
			}
		

		bool inPosition();
		void move(float panCordinate, float tiltCordinate);
		void stop();
		void runUp();
		void runDown();
		void runLeft();
		void runRight();
	private:
		AngularSensor 		_sensorTilt;
		AngularSensor 		_sensorPan;

		AccelStepper 		_stepperTilt;
		AccelStepper 		_stepperPan;

		float 				_speedTilt;
		float 				_speed;
		int					_gearRatio;

		bool				_tiltInPosition;
		bool				_panInPosition;

		float				_satAngle;
		float				_currentPanCordinate;
		float				_currentTiltCordinate;

		void pan(float cordinate);
		void tilt(float cordinate);
		float calcSteppsToMove(float degs);
		float pathToMove(float cordinate, float currentCordinate);
};

#endif
