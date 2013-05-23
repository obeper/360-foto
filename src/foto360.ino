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
#include "Bluetooth.h"

#include "AngularSensor.h"

enum ProgramState {PGM_SETUP, PGM_READY,PGM_STARTED, PGM_RUNNING, CAMERA_IN_POSITION, HDR_IS_TAKEN, PGM_DONE, PGM_PAUSED, PGM_RESUMED, PGM_RESET, PGM_BT_RUN_MOTORS};
enum ProcessEvent {EVT_NOTHING, EVT_NEW, EVT_RUN_MOTORS, EVT_CHANGE_CAMERA_PROPERTY, EVT_TAKE_PHOTO, EVT_LAST_PICTURE_IS_TAKEN};
enum CameraEvent {EVT_BT_NOTHING, EVT_BT_CHANGE_PROP, EVT_BT_TAKE_PHOTO, EVT_BT_RUN_MOTORS, EVT_SETUP_CAMERA_SETTINGS};
//////////////////////////////////////////////
////////////////////MAINPROGRAM///////////////
//////////////////////////////////////////////

//DEFINE MAIN OBJECTS AND CONSTANTS

#define BATTERY_READ_INTERVAL       5000       // mSEC
#define BATTERY_UPDATE_INTERVAL     20000       // mSEC
#define STARTING_DELAY_TIMER        6           // SEC
#define RESUME_DELAY_TIMER          6           // SEC
#define BUTTON_DELAY_TIMER          1500        // mSEC
#define RESET_DELAY_TIMER           5000        // mSEC
#define BUTTON_DELAY_TO_RESET       2500        // mSEC
#define BT_SHOW_INFO_SCREEN_DELAY   3000        // mSEC



//int tiltReadAnalogPin, int tiltStepperDirPin, int tiltStepperStepPin,
//int panReadAnalogPin, int panStepperDirPin, int panStepperStepPin
RotateCamera camera( 1, 26, 9, 0 , 25, 10);


//int rs, int enable,int d4, int d5, int d6, int d7)
MainDisplay display(22,13,12,23,11,24);

BatterySensor battery(2);


Bluetooth bluetooth(6);
//Camera 15.0 brännvidd, sensorH 22.3, sensorV 14.3, overlap 1.2
PanoramaSettings panoramaSettings(15.0, 22.3, 14.9, 0.2);
ProgramState currentState;


//USERINTERFACE PINS
int startButtonPin;
int statusLedRGB[3] = {4,5,3};

//TIMERS
int batteryPercentage;
unsigned long lastBatteryReadTime; 
unsigned long lastBatteryUpdateTime; 
unsigned long lastTimeToStartTime;
unsigned long lastTimeButtonWasPressed;
unsigned long lastTimeToResetTime;
unsigned long lastTimeTimeLeftWasUpdated;
unsigned long buttonPressedTime;
unsigned long currentTime;
unsigned long lastTimeBTPropertyChanged;


//CONTROL GLOBALS
bool wasHigh;
bool startByBluetooth;
int timeToStartSec;




//DEBUG
unsigned long lastDebugTime;

//FOTO SETTINGS
int cameraPropertyName;
int cameraPropertyValue;

//BT
int btMotorDirection = 0;

//FOTOPROCESS VARIABLES
int currentPictureNr;
int lastPictureNr;
int timeLeftOfProcess;
int pictureNrInCurrentHDR;
int evValue;
int shutterSpeedValue;

bool cameraPhotoIsTaken;
bool cameraPropertyChanged;
bool cameraConnected;
bool cameraSetupComplete;

//EVENT ENUMS
ProcessEvent currentProcessEvent;
CameraEvent currentCameraEvent;

//CAMERA CONTROL CLASS
class CamStateHandlers : public PTPStateHandlers
{
      bool stateConnected;
      int setupPropertyCounter;
    
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
        cameraConnected = false;
        Notify(PSTR("Camera disconnected\r\n"),0x80);
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp)
{
    static unsigned long next_time = 0;
    
    if (!stateConnected){
        setupPropertyCounter = 0;
        cameraConnected = true;
        stateConnected = true;
    }

    unsigned long  time_now = millis();

    if (time_now > next_time)
    {
        next_time = time_now + 300;
        
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
        }else if(currentCameraEvent == EVT_BT_CHANGE_PROP){
            uint16_t rc2 = Eos.SetProperty(cameraPropertyName, cameraPropertyValue);
            if(rc2 == PTP_RC_OK){
                 bluetooth.updated();
                 currentCameraEvent = EVT_BT_NOTHING;
            }
        }else if (currentCameraEvent == EVT_BT_TAKE_PHOTO){
            uint16_t rc = Eos.Capture();
             if (rc == PTP_RC_OK){
                bluetooth.updated();
                currentCameraEvent = EVT_BT_NOTHING;
             }
        }else if (currentCameraEvent == EVT_SETUP_CAMERA_SETTINGS){
            int properties[5] = {EOS_DPC_Iso, EOS_DPC_Aperture, EOS_DPC_WhiteBalance, EOS_DPC_AFMode, EOS_DPC_DriveMode};
            int propVal[5] = {72, 386, 1, 0, 0};

            uint16_t rc = Eos.SetProperty(properties[setupPropertyCounter], propVal[setupPropertyCounter]);
            if(rc == PTP_RC_OK){
                setupPropertyCounter++;
            }
            if(setupPropertyCounter == 5){
                setupPropertyCounter = 0;
                cameraSetupComplete = true;
                currentCameraEvent = EVT_BT_NOTHING;
            }
            
        }
    }

}

void setup()
{
    Serial.begin(9600);

    //START BLUETOOTH
    Serial2.begin(115200);


    int timeNow = millis();


    display.onScreen();


    //START USB 
     if (Usb.Init() == -1)
        Serial.println("OSC did not start.");


    //PGM DEFAULTS
    currentState = PGM_SETUP;
    currentProcessEvent = EVT_NOTHING;



    startButtonPin = 2;
    pinMode(startButtonPin, INPUT);
    pinMode(statusLedRGB[0], OUTPUT);
    pinMode(statusLedRGB[1], OUTPUT);
    pinMode(statusLedRGB[2], OUTPUT);

    //RESET ALL TIMERS
    lastBatteryReadTime = timeNow;
    lastTimeToStartTime = timeNow;
    lastTimeButtonWasPressed = timeNow;
    lastTimeToResetTime = timeNow;
    lastDebugTime = timeNow;
    lastBatteryUpdateTime = timeNow;
    lastTimeTimeLeftWasUpdated = timeNow;
    lastTimeBTPropertyChanged = timeNow;
    
    buttonPressedTime = 0;
    wasHigh = false;

    timeToStartSec = STARTING_DELAY_TIMER;
    currentPictureNr = 0;
    lastPictureNr = 0;
    
    //Statndard väden som panoramainställningar
        shutterSpeedValue = 386;
        panoramaSettings.setPlusMinusEv(3);
        panoramaSettings.setMiddShutterSpeed(shutterSpeedValue);

    cameraSetupComplete = false;
    cameraConnected = false;
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
    

/*
    if(currentTime > (1000 + lastDebugTime)){

        Serial.println("--------------");
        Serial.println("Current State:");
        Serial.println(currentState);
        Serial.println("Process State");
        Serial.println(currentProcessEvent);
        Serial.println("ShutterSpeed:");
        Serial.println(cameraConnected);
        lastDebugTime = currentTime;
    }
*/


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


    
    //WHAT TO LOOP - CHECK PRPGRAM_STATE
    switch(currentState){
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_SETUP:
            
            display.connectCameraScreen();
            Usb.Task();
            if(cameraConnected){
              currentState = PGM_READY;  
            }
            
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_READY:

            analogWrite(statusLedRGB[1], 15);
            analogWrite(statusLedRGB[0], 0);
            if(currentTime > (lastTimeBTPropertyChanged + BT_SHOW_INFO_SCREEN_DELAY)){
                display.readyScreen(batteryPercentage);
            }
            
            Usb.Task();
            if(!cameraConnected){
                currentState = PGM_SETUP;
            }else if(!cameraSetupComplete){
                currentCameraEvent = EVT_SETUP_CAMERA_SETTINGS; 
            }


            //CHECK START/STOP BUTTON PRESS
            if(digitalRead(startButtonPin) == HIGH || startByBluetooth){
                startByBluetooth = false;
                currentState = PGM_STARTED;
                analogWrite(statusLedRGB[0],15);
                analogWrite(statusLedRGB[1],0);
                // Zero Process variables
                currentPictureNr = 0;
                lastPictureNr = 0;
                cameraPropertyChanged = false;
                cameraPhotoIsTaken = false;
                currentProcessEvent = EVT_NEW;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

        case PGM_STARTED:
            
            //COUNTDOWN TO START
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

            //Check button for PAUS-ACTION
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
                    // INIIT EVERTHING FOR NEW PROCESS
                    currentCameraEvent = EVT_BT_NOTHING;
                    timeLeftOfProcess = 4000;
                    currentPictureNr = 1;
                    pictureNrInCurrentHDR = 1;
      
                    evValue = panoramaSettings.getPlusMinusEv();
                    lastPictureNr = panoramaSettings.getNrOfPictures();
                    currentProcessEvent = EVT_RUN_MOTORS;
                break;
                case EVT_RUN_MOTORS:
                    //MOVE CAMERA TO array cordinates(currentPictureNr)
                
                    camera.move(panoramaSettings.calcPanCordinate(currentPictureNr-1), 
                                panoramaSettings.calcTiltCordinate(currentPictureNr-1));

                   
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
                        if(currentPictureNr >= lastPictureNr){
                            currentState = PGM_DONE;
                        }else{
                            pictureNrInCurrentHDR = 1;
                            currentPictureNr++;
                            currentProcessEvent = EVT_RUN_MOTORS;
                        }
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
            currentProcessEvent = EVT_NOTHING;
            if(currentTime > lastTimeToResetTime + RESET_DELAY_TIMER ){
                currentState = PGM_SETUP;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        case PGM_DONE:
            display.doneScreen(currentPictureNr,lastPictureNr,batteryPercentage,timeLeftOfProcess);
            //CHECK START/STOP BUTTON PRESS
            if(digitalRead(startButtonPin) == HIGH){
                currentState = PGM_READY;
            }
            break;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
        }


            //CHECK BLUETOOTH
            if(bluetooth.newDataToRead()){
                bluetooth.handleAction();

                if(bluetooth.updateShutterVariable()  && currentState == PGM_READY){
                    display.propertyChangedScreen(batteryPercentage,"SS CHANGED");
                    lastTimeBTPropertyChanged = currentTime;
                    panoramaSettings.setMiddShutterSpeed(bluetooth.readPropertyValue());
                    bluetooth.updated();
                }else if(bluetooth.updateEvVariable() && currentState == PGM_READY){
                    display.propertyChangedScreen(batteryPercentage,"EV CHANGED");
                    lastTimeBTPropertyChanged = currentTime;
                    panoramaSettings.setPlusMinusEv(bluetooth.readPropertyValue());
                    bluetooth.updated();
                }else if(bluetooth.updateFocalLength() && currentState == PGM_READY){
                    display.propertyChangedScreen(batteryPercentage,"FL CHANGED");
                    lastTimeBTPropertyChanged = currentTime;
                    panoramaSettings.setFocalLength(bluetooth.readPropertyValue());
                    bluetooth.updated();
                }else if(bluetooth.updateCameraSetting() && currentState == PGM_READY){
                    display.propertyChangedScreen(batteryPercentage,"PROPERTY CHANGED");
                    lastTimeBTPropertyChanged = currentTime;
                    currentCameraEvent = EVT_BT_CHANGE_PROP;
                    cameraPropertyName = bluetooth.readPropertyName();
                    cameraPropertyValue = bluetooth.readPropertyValue();
                }else if(bluetooth.shouldTakePicture() && currentState == PGM_READY){
                    currentCameraEvent = EVT_BT_TAKE_PHOTO;
                }else if(bluetooth.startStopProgram()){
                    if(currentState == PGM_RUNNING){
                        currentState = PGM_PAUSED;
                    }else if(currentState == PGM_READY){
                        startByBluetooth = true;
                    }else if(currentState == PGM_PAUSED){
                        currentState = PGM_RESUMED; 
                    }
                    bluetooth.updated();
                }else if(bluetooth.moveCameraUp() && currentState == PGM_READY){
                    currentCameraEvent = EVT_BT_RUN_MOTORS;
                    btMotorDirection = 1;
                }else if(bluetooth.moveCameraDown() && currentState == PGM_READY){
                    currentCameraEvent = EVT_BT_RUN_MOTORS;
                    btMotorDirection = 2;
                }else if(bluetooth.moveCameraLeft() && currentState == PGM_READY){
                    currentCameraEvent = EVT_BT_RUN_MOTORS;
                    btMotorDirection = 3;
                }else if(bluetooth.moveCameraRight() && currentState == PGM_READY){
                    currentCameraEvent = EVT_BT_RUN_MOTORS;
                    btMotorDirection = 4;
                }else if(bluetooth.moveCameraHome() && currentState == PGM_READY){
                        currentCameraEvent = EVT_BT_RUN_MOTORS;
                        btMotorDirection = -1;   
                }else if(bluetooth.moveCameraStop() && currentState == PGM_READY){
                        currentCameraEvent = EVT_BT_RUN_MOTORS;
                        btMotorDirection = 0;   
                }
                delay(1);
            }
        ///MANUAL CONTROL OF MOTORS
        if(currentCameraEvent == EVT_BT_RUN_MOTORS){
            switch(btMotorDirection){
                case -1:
                    camera.move(0.0, 45.0);
                    if(camera.inPosition()){
                        currentCameraEvent = EVT_BT_NOTHING;
                        bluetooth.updated();
                    }
                break;
                case 0:
                    camera.stop();
                    bluetooth.updated();
                    currentCameraEvent = EVT_BT_NOTHING;
                break;
                case 1:
                //UP
                    camera.runUp();
                break;

                case 2:
                //Down
                    camera.runDown();
                break;
                case 3:
                //Left
                    camera.runLeft();
                break;
                case 4:
                //RIGHT
                    camera.runRight();
                break;

            }
        }
}