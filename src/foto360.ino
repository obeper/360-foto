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


#include "MainDisplay.h"
#include "AccelStepper.h"
#include "Arduino.h"
#include "RotateCamera.h"
#include "BatterySensor.h"


enum ProgramState {PGM_READY,PGM_STARTED, PGM_RUNNING, CAMERA_IN_POSITION, HDR_IS_TAKEN, PGM_DONE, PGM_PAUSED, PGM_RESUMED, PGM_ABORTED};
enum ProgramEvent {EVT_NOTHING, EVT_START_IS_PRESSED, EVT_BLUETOOTH_IS_ACTIVE};

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




//////////////////////////////////////////////
////////////////////MAINPROGRAM///////////////
//////////////////////////////////////////////

//DEFINE MAIN OBJECTS AND CONSTANTS

#define BATTERY_READ_INTERVAL   60000       // mSEC
#define STARTING_DELAY_TIMER    6           // SEC
#define RESUME_DELAY_TIMER      6           // SEC
#define BUTTON_DELAY_TIMER      1500        // mSEC
//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin



RotateCamera camera(0, 25, 10, 2, 26, 9);
//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(8);

ProgramState currentState;

int analogStartButtonPin = 1;
//Mainfunktion globals
int batteryPercentage;
unsigned long lastBatteryReadTime; 
unsigned long lastTimeToStartTime;
unsigned long lastTimeButtonWasPressed;

int timeToStartSec;
int currentPictureNr;
int lastPictureNr;
unsigned long currentTime;

//DEBUG
unsigned long lastDebugTime;

void setup()
{
    Serial.begin(9600);
    int timeNow = millis();

    display.onScreen();

    currentState = PGM_READY;

    batteryPercentage = battery.readPercentage();
    lastBatteryReadTime = timeNow;
    lastTimeToStartTime = timeNow;
    lastTimeButtonWasPressed = timeNow;
    lastDebugTime = timeNow;

    timeToStartSec = STARTING_DELAY_TIMER;
    currentPictureNr = 0;
    lastPictureNr = 0;

    display.setRefreshRate(800);
    
    delay(2000);
}
void loop()
{
  
    currentTime = millis();
    


    if(currentTime > (1000 + lastDebugTime)){
         Serial.println(currentState);
         lastDebugTime = currentTime;
    }



    //READ BATTERY ONCE EVERY 60 SEC (To avoid small fluctuations)
    if(currentTime > (lastBatteryReadTime + BATTERY_READ_INTERVAL)){
        batteryPercentage = battery.readPercentage();
        lastBatteryReadTime = currentTime;
    }

    //CHECK BLUETOOTH
        //TODO !!!

    switch(currentState){

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_READY:
            display.readyScreen(batteryPercentage);
            //CHECK START/STOP BUTTON PRESS
            if(analogRead(analogStartButtonPin) > 1000){
                currentState = PGM_STARTED;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

        case PGM_STARTED:
            
            if(currentTime > lastTimeToStartTime + 1000){
                display.startScreen(timeToStartSec);
                lastTimeToStartTime = currentTime;
                timeToStartSec--;

                if(timeToStartSec == 0){
                    timeToStartSec = STARTING_DELAY_TIMER;
                    currentState = PGM_RUNNING;
                    lastTimeToStartTime = 0;
                }
            }
            
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_RUNNING:
            if(analogRead(analogStartButtonPin) > 1000 && currentTime > (lastTimeButtonWasPressed + BUTTON_DELAY_TIMER )){
                currentState = PGM_PAUSED;
                lastTimeButtonWasPressed = currentTime;
            }
            display.runningScreen(7,24,batteryPercentage,500);
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_PAUSED:
            if(analogRead(analogStartButtonPin) > 1000 && currentTime > (lastTimeButtonWasPressed + BUTTON_DELAY_TIMER )){
                currentState = PGM_RESUMED;
                lastTimeButtonWasPressed = currentTime;
            }
            display.pauseScreen(7,24,batteryPercentage,500);
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_RESUMED:
            if(currentTime > lastTimeToStartTime + 1000){
                display.resumeScreen(timeToStartSec);
                lastTimeToStartTime = currentTime;
                timeToStartSec--;
                
                if(timeToStartSec == 0){
                    timeToStartSec = RESUME_DELAY_TIMER;
                    currentState = PGM_RUNNING;
                    lastTimeToStartTime = 0;
                }
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        }

}