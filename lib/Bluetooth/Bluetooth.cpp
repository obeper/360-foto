#include "Bluetooth.h"
#include "Arduino.h"
#include "canoneos.h"

Bluetooth::Bluetooth(int blueLedPin){
  _blueLedPin = blueLedPin;
  pinMode(_blueLedPin,OUTPUT);
}
//Public
bool Bluetooth::newDataToRead(){
  return (bool)Serial2.available();
}
void Bluetooth::updated(){
  _updateCameraSettings = false;
  _updateEvVariable = false;
  _updateShutterVariable = false;
  _shouldStart = false;
  _shouldMoveCamera = false;
  _shouldTakePicture = false;

}
void Bluetooth::handleAction(){
    char queryType;
    queryType = Serial2.read();
    _action = queryType;
    delay(1);
  switch(_action){
      case 'Z':
        this->handleStatusLed();
      break;
      case 'A':
        _propertyValue =  this->readFloatSerial();
        _updateCameraSettings = true;
        _propertyName = EOS_DPC_Aperture;
      break;
      case 'W':
        _propertyValue =  this->readFloatSerial();
        _propertyName = EOS_DPC_WhiteBalance;
        _updateCameraSettings = true;
      break;
       case 'I':
        _propertyValue =  this->readFloatSerial();
        _propertyName = EOS_DPC_Iso;
        _updateCameraSettings = true;
      break;
       case 'E':
        _propertyValue =  this->readFloatSerial();
        _updateEvVariable = true;
      break;
       case 'S':
        _propertyValue =  this->readFloatSerial();
        _updateShutterVariable = true;
      break;
      case 'C':
        _shouldTakePicture = true;
      break;
      case 'P':
        _shouldStart = true;
      break;
    }

}

bool Bluetooth::updateCameraSetting(){
  return _updateCameraSettings;
}
bool Bluetooth::updateEvVariable(){
  return _updateEvVariable;
}
bool Bluetooth::updateShutterVariable(){
  return _updateShutterVariable;
}
bool Bluetooth::startStopProgram(){
  return _shouldStart;
}
bool Bluetooth::moveCamera(){
  return _shouldMoveCamera;
}
bool Bluetooth::shouldTakePicture(){
  return _shouldTakePicture;
}

int Bluetooth::readPropertyValue(){
 return (int)_propertyValue;
}
int Bluetooth::readPropertyName(){
    return _propertyName;
  }

//Private

void Bluetooth::handleStatusLed(){
  char connectionStatus;
  connectionStatus = Serial2.read();
  if(connectionStatus == 'C'){
    analogWrite(_blueLedPin,10);
  }else{
    analogWrite(_blueLedPin,0);
  }
  //FLUSH PORT FROM CRAP 
  while(this->newDataToRead()){
    connectionStatus = Serial2.read();
  }
}
float Bluetooth::readFloatSerial() {
  byte index = 0;
  byte timeout = 0;
  char data[128] = "";

  do {
    if (Serial2.available() == 0) {
      delay(1);
      timeout++;
    }
    else {
      data[index] = Serial2.read();
      timeout = 0;
      index++;
    }
  }  
  while ((data[constrain(index-1, 0, 128)] != ';') && (timeout < 5) && (index < 128));
  return atof(data);
}
