#include "MainDisplay.h"


//CONSTRUCTOR
MainDisplay::MainDisplay(int rs, int enable,int d4, int d5, int d6, int d7) : 
LiquidCrystal(rs, enable, d4, d5, d6, d7)
{
	//No state
	_width = 16;
	_height = 2;
	_refreshRate = 500;
		_lastScreenUpdate = millis();
	this->begin(_width, _height);
}
//PUBLIC

void MainDisplay::onScreen(){
	
	this->clear();
	this->printCenterUp("GLOBETROTTER");

	
	
}
void MainDisplay::readyScreen(int batteryStatus){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printLeftUp("Ready");
	this->printRigthUp((String)batteryStatus + "%");
	_lastScreenUpdate = millis();
}
void MainDisplay::connectCameraScreen(){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printCenterUp("Connect");
	this->printCenterDown("Cannon EOS");
	_lastScreenUpdate = millis();
}
void MainDisplay::receivingDataScreen(){
	if(!this->shouldRefresh())return;
		this->clear();
		this->printCenterUp("GLOBETROTTER");
		this->printCenterDown("Receiving data");
	_lastScreenUpdate = millis();
}
void MainDisplay::startScreen(int timeToStart){
	if(!this->shouldRefresh())return;
	this->clear();
	if(timeToStart >= 10){
		this->printCenterUp((String)timeToStart+"s to Start");
	}else{
		String t = (String)timeToStart+"s to Start";
		this->setCursor((int)((_width -  t.length()) / 2 +1), 0);
		this->print(t);
	}
	_lastScreenUpdate = millis();}
void MainDisplay::runningScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printLeftUp("Running");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_lastScreenUpdate = millis();	
}
void MainDisplay::pauseScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printLeftUp("Paused");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_lastScreenUpdate = millis();	
}
void MainDisplay::doneScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printLeftUp("Done");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_lastScreenUpdate = millis();	
}
void MainDisplay::resettingScreen(){
	if(!this->shouldRefresh())return;
	this->clear();
	this->printCenterUp("GLOBETROTTER");
	this->printCenterDown("Resetting");
	_lastScreenUpdate = millis();
}
void MainDisplay::resumeScreen(int timeToResume){
	if(!this->shouldRefresh())return;
	this->clear();
	if(timeToResume >= 10){
		this->printCenterUp((String)timeToResume+"s to Resume");
	}else{
		String t = (String)timeToResume+"s to Resume";
		this->setCursor((int)((_width -  t.length()) / 2 +1), 0);
		this->print(t);
	}
	_lastScreenUpdate = millis();
}
void MainDisplay::setRefreshRate(unsigned long refreshRate){
	this->_refreshRate = refreshRate;
}
//PRIVATE
void MainDisplay::printCenterUp(String wordToPrint){
	this->setCursor((int)((_width -  wordToPrint.length()) / 2), 0);
	this->print(wordToPrint);
}

void MainDisplay::printCenterDown(String wordToPrint){
	this->setCursor((int)((_width -  wordToPrint.length()) / 2), 1);
	this->print(wordToPrint);
}	

void MainDisplay::printLeftUp(String wordToPrint){
	this->setCursor(0, 0);
	this->print(wordToPrint);
}
void MainDisplay::printLeftDown(String wordToPrint){
	this->setCursor(0, _height - 1);
	this->print(wordToPrint);
}
void MainDisplay::printRigthUp(String wordToPrint){
	this->setCursor(_width  - wordToPrint.length(), 0);
	this->print(wordToPrint);
}
void MainDisplay::printRigthDown(String wordToPrint){
	this->setCursor(_width  - wordToPrint.length(),	1);
	this->print(wordToPrint);

}
bool MainDisplay::shouldRefresh(){
	return millis() > (	_lastScreenUpdate + _refreshRate);
}