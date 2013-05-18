#include "Bluetooth.h"
#include "Arduino.h"


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
        this->_updateCameraSettings = true;
      break;
       case 'W':
        _propertyValue =  this->readFloatSerial();
        this->_updateCameraSettings = true;
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
        Serial.println("Capture");
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

int Bluetooth::readPropertyValue(){
 return (int)_propertyValue;
}
int Bluetooth::readPropertyName(){
    return _propertyName;
  }
bool Bluetooth::shouldStart(){
  return _shouldStart;
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
  //FLUSH PORT 
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
