#include "Arduino.h"
#include "RotateCamera.h"
#include "MainDisplay.h"
#include "BatterySensor.h"
//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin
RotateCamera camera(2, 26, 9, 0, 25, 10);
//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(8);

bool readyForStart;



int startButtonPin = 2;
void setup() {

	//BEGIN BLUETOOTH
	Serial2.begin(115200);
	display.onScreen();
	delay(3000);
	readyForStart = true;

	//
	pinMode(startButtonPin,INPUT);	
}

void loop() {


	if(readyForStart){
		display.readyScreen(battery.readPercentage());
		readyForStart = false;
	}

	if(digitalRead(startButtonPin) == HIGH){

		//START
		int timeToStart = 5;
		for (int i = timeToStart; i > 0; --i)
		{
			display.startScreen(i);
			delay(1000);
		}
		display.runningScreen(2,5,battery.readPercentage(),200);
		delay(3000);
		readyForStart = true;

	}
	
}