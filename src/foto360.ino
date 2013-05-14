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

#include "AccelStepper.h"

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
//RotateCamera camera(2, 26, 9, 0, 25, 10);
RotateCamera camera(0, 25, 10, 2, 26, 9);
//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(8);

enum ProgramState {READY, }


void setup()
{ 

}
void loop()
{
    camera.move(180,30);
    if(!screenOn){
        display.onScreen();
        screenOn = true;
    }
    

}