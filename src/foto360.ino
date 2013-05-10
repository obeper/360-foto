#include <inttypes.h>
#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <address.h>

#include <message.h>

#include <ptp.h>
#include <canoneos.h>


#include "Arduino.h"
#include "RotateCamera.h"
#include "MainDisplay.h"
#include "BatterySensor.h"


class CamStateHandlers : public PTPStateHandlers
{
      bool stateConnected;
    
public:
      CamStateHandlers() : stateConnected(false) {};
      
      virtual void OnDeviceDisconnectedState(PTP *ptp);
      virtual void OnDeviceInitializedState(PTP *ptp);
} CamStates;

USB         Usb;
USBHub      Hub1(&Usb);
CanonEOS    Eos(&Usb, &CamStates);
bool pictureIsTaken;

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp)
{
    if (stateConnected)
    {
        stateConnected = false;
        Notify(PSTR("Camera disconnected\r\n"),0x80);
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp)
{
    static uint32_t next_time = 0;
    
    if (!stateConnected)
        stateConnected = true;

    uint32_t  time_now = millis();

    if (time_now > next_time)
    {
        next_time = time_now + 3000;
        
        uint16_t rc = Eos.Capture();
        pictureIsTaken = true;
    
        if (rc != PTP_RC_OK)
            ErrorMessage<uint16_t>("Error", rc);
    }
}

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
    //Serial2.begin(115200);
    Serial.begin(115200);
    display.onScreen();
    delay(3000);

    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

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
        camera.move(30,30);
        pictureIsTaken = false; 
        while(!pictureIsTaken){
            Usb.Task();
        }
        

    }

    
}