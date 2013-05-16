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
#include "PanoramaSettings.h"

enum ProgramState {PGM_SETUP, PGM_READY,PGM_STARTED, PGM_RUNNING, CAMERA_IN_POSITION, HDR_IS_TAKEN, PGM_DONE, PGM_PAUSED, PGM_RESUMED, PGM_RESET};
enum ProcessEvent {EVT_NOTHING, EVT_NEW, EVT_RUN_MOTORS, EVT_CHANGE_CAMERA_PROPERTY, EVT_TAKE_PHOTO, EVT_BLUETOOTH_IS_ACTIVE, EVT_LAST_PICTURE_IS_TAKEN};

//////////////////////////////////////////////
////////////////////MAINPROGRAM///////////////
//////////////////////////////////////////////

//DEFINE MAIN OBJECTS AND CONSTANTS

#define BATTERY_READ_INTERVAL   5000       // mSEC
#define BATTERY_UPDATE_INTERVAL 20000       // mSEC
#define STARTING_DELAY_TIMER    6           // SEC
#define RESUME_DELAY_TIMER      6           // SEC
#define BUTTON_DELAY_TIMER      1500        // mSEC
#define RESET_DELAY_TIMER       5000        // mSEC
#define BUTTON_DELAY_TO_RESET   3000        // mSEC



//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin
RotateCamera camera(0, 25, 10, 2, 26, 9);

//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(8);

//Camera 15.0 brännvidd, sensorH 22.3, sensorV 14.3, overlap 1.2
PanoramaSettings panoramaSettings(15.0, 22.3, 14.9, 1.2);
ProgramState currentState;

int startButtonPin;


int batteryPercentage;
unsigned long lastBatteryReadTime; 
unsigned long lastBatteryUpdateTime; 
unsigned long lastTimeToStartTime;
unsigned long lastTimeButtonWasPressed;
unsigned long lastTimeToResetTime;
unsigned long lastTimeTimeLeftWasUpdated;

unsigned long buttonPressedTime;
bool wasHigh;

int timeToStartSec;

unsigned long currentTime;


//DEBUG
unsigned long lastDebugTime;


//FOTOPROCESS VARIABLES
int currentPictureNr;
int lastPictureNr;
int timeLeftOfProcess;
int pictureNrInCurrentHDR;
int evValue;
int shutterSpeedValue;

bool cameraPhotoIsTaken;
bool cameraPropertyChanged;
ProcessEvent currentProcessEvent;


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
    static unsigned long next_time = 0;
    
    if (!stateConnected)
        stateConnected = true;

    unsigned long  time_now = millis();

    if (time_now > next_time)
    {
        next_time = time_now + 200;
        
        if(currentProcessEvent == EVT_TAKE_PHOTO && !cameraPhotoIsTaken){
             uint16_t rc = Eos.Capture();
             if (rc == PTP_RC_OK){
                cameraPhotoIsTaken = true;
             }
            
        }else if(currentProcessEvent == EVT_CHANGE_CAMERA_PROPERTY && !cameraPropertyChanged){
            uint16_t rc2 = Eos.SetProperty(EOS_DPC_ShutterSpeed, shutterSpeedValue);
            if(rc2 == PTP_RC_OK){
                cameraPropertyChanged = true;
            }
        }
       
    
        
    }
}






void setup()
{
    Serial.begin(9600);
    int timeNow = millis();

    display.onScreen();

    currentState = PGM_SETUP;
    currentProcessEvent = EVT_NOTHING;

    startButtonPin = 2;
    pinMode(startButtonPin, INPUT);

    lastBatteryReadTime = timeNow;
    lastTimeToStartTime = timeNow;
    lastTimeButtonWasPressed = timeNow;
    lastTimeToResetTime = timeNow;
    lastDebugTime = timeNow;
    lastBatteryUpdateTime = timeNow;
    lastTimeTimeLeftWasUpdated = timeNow;
    
    buttonPressedTime = 0;
    wasHigh = false;

    timeToStartSec = STARTING_DELAY_TIMER;
    currentPictureNr = 0;
    lastPictureNr = 0;


    display.setRefreshRate(800);
    
    delay(1400);

    battery.readVoltage();
    delay(200);
    battery.readVoltage();
    delay(200);
    battery.readVoltage();
    delay(200);
    batteryPercentage = battery.readPercentage();
}
void loop()
{
  
    currentTime = millis();
    


    if(currentTime > (1000 + lastDebugTime)){

        Serial.println("--------------");
        Serial.println("Current State:");
        Serial.println(currentState);
        Serial.println("Process State");
        Serial.println(currentProcessEvent);
        Serial.println("ShutterSpeed:");
        Serial.println(shutterSpeedValue);
        lastDebugTime = currentTime;
    }



    //READ BATTERY ONCE EVERY 5 SEC TO GET MEAN VALUE 
    if(currentTime > (lastBatteryReadTime + BATTERY_READ_INTERVAL)){
       battery.readVoltage();
        lastBatteryReadTime = currentTime;
    }
    //UPDATE BATTERY ONCE EVERY 5 SEC 
    if(currentTime > (lastBatteryUpdateTime + BATTERY_UPDATE_INTERVAL)){
       batteryPercentage = battery.readPercentage();
        lastBatteryUpdateTime = currentTime;
    }


    

    switch(currentState){
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_SETUP:
            currentPictureNr = 0;
            lastPictureNr = 0;
            timeLeftOfProcess = 4000;
            currentProcessEvent = EVT_NEW;
            shutterSpeedValue = 27;
            evValue = 1;
            cameraPropertyChanged = false;
            cameraPhotoIsTaken = false;

            currentState = PGM_READY;
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_READY:
            display.readyScreen(batteryPercentage);
            //CHECK BLUETOOTH
            if(Serial2.available()){
            //TO FUNKY SHIT WITH BLUETOOTH
            }
            //CHECK START/STOP BUTTON PRESS
            if(digitalRead(startButtonPin) == HIGH){
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
            //Check button
            if(digitalRead(startButtonPin) == HIGH && currentTime > (lastTimeButtonWasPressed + BUTTON_DELAY_TIMER )){
                currentState = PGM_PAUSED;
                lastTimeButtonWasPressed = currentTime;
            }
            //Decrease time left
            if(currentTime > (lastTimeTimeLeftWasUpdated + 1000)){
                timeLeftOfProcess -= 1;
                lastTimeTimeLeftWasUpdated = currentTime;
            }
            display.runningScreen(currentPictureNr,lastPictureNr,batteryPercentage,timeLeftOfProcess);
            switch(currentProcessEvent){
                case EVT_NEW:
                    //CALC CAMERA CORDINATES - NR OF PICTURES
                    currentPictureNr = 1;
                    pictureNrInCurrentHDR = 1;
                    evValue = panoramaSettings.getPlusMinusEv();
                    lastPictureNr = panoramaSettings.getNrOfPictures();
                    //SET EVT_RUN MOTORS
                    currentProcessEvent = EVT_RUN_MOTORS;
                break;
                case EVT_RUN_MOTORS:
                    //MOVE CAMERA TO array cordinates(currentPictureNr)
                    camera.move(panoramaSettings.calcPanCordinate(currentPictureNr), 
                                panoramaSettings.calcTiltCordinate(currentPictureNr));

                    if(camera.inPosition()){
                        currentProcessEvent = EVT_CHANGE_CAMERA_PROPERTY;
                    }
                    //Check for cameraInPosition and set evt_CHANGE_CAMERA_PROPERTY
                break;
                case EVT_CHANGE_CAMERA_PROPERTY:
                    if(pictureNrInCurrentHDR <= 2*evValue +1){
                        int currentEv = -1*evValue + pictureNrInCurrentHDR - 1;
                        shutterSpeedValue = panoramaSettings.getShutterSpeed(currentEv);

                        cameraPropertyChanged = false;
                        Usb.Task();
                        if(cameraPropertyChanged){
                            cameraPhotoIsTaken = false;
                            currentProcessEvent = EVT_TAKE_PHOTO;
                        }
                    }else{
                        currentPictureNr++;
                        currentProcessEvent = EVT_RUN_MOTORS;
                    }

                    
                    // SET WHAT SHOULD CHANGE AND SPAM Usb.Task();
                break;
                case EVT_TAKE_PHOTO:
                    // SPAM TAKE PHOTO Usb.Task();
                    Usb.Task();
                    if(cameraPhotoIsTaken){
                        pictureNrInCurrentHDR++;
                        currentProcessEvent = EVT_CHANGE_CAMERA_PROPERTY;
                    }
                break;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_PAUSED:
            if(digitalRead(startButtonPin) == HIGH && currentTime > (lastTimeButtonWasPressed + BUTTON_DELAY_TIMER )){
                if(!wasHigh){
                    buttonPressedTime = currentTime;     
                }
                wasHigh = true;

            }
            if(digitalRead(startButtonPin) == LOW && wasHigh){
                buttonPressedTime = currentTime - buttonPressedTime;
                if (buttonPressedTime < BUTTON_DELAY_TO_RESET)
                {
                    currentState = PGM_RESUMED;
                    
                }
                wasHigh = false;
                lastTimeButtonWasPressed = currentTime;
            }
            if(digitalRead(startButtonPin) == HIGH && wasHigh && currentTime > ( buttonPressedTime + BUTTON_DELAY_TO_RESET) ){
                currentState = PGM_RESET;
                lastTimeToResetTime = currentTime;
                wasHigh = false;
                lastTimeButtonWasPressed = currentTime;
            }

            display.pauseScreen(currentPictureNr,lastPictureNr,batteryPercentage,timeLeftOfProcess);
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
        case PGM_RESET:
            display.resettingScreen();
            //NOLLSTÄLLA ALLT
            if(currentTime > lastTimeToResetTime + RESET_DELAY_TIMER ){
                currentState = PGM_SETUP;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        }

}