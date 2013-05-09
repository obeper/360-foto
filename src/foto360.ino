#include "Arduino.h"
#include "RotateCamera.h"
#include "MainDisplay.h"
#include "BatterySensor.h"
//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin
//RotateCamera camera(2, 26, 9, 0, 25, 10);
//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(8);


void setup() {
	Serial2.begin(115200);
	pinMode(7,OUTPUT);
}

void loop() {
	

	if(Serial2.available()){
		char val = Serial2.read();
		if(val = 'S'){
			digitalWrite(7,HIGH);
		}
		delay(1500);
		digitalWrite(7,LOW);
	}
	int batPercenage = battery.readPercentage();

	display.pauseScreen(12,13,batPercenage,213);
	delay(2000);
	
  
}