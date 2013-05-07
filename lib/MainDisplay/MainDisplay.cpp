#include "MainDisplay.h"

//enum displayState { BLANK, ON, READY, RECEIVINGDATA, START, RUNNING, PAUSE, RESUME, RESET, DONE  };


//CONSTRUCTOR
MainDisplay::MainDisplay(int rs, int enable,int d4, int d5, int d6, int d7) : 
LiquidCrystal(rs, enable, d4, d5, d6, d7)
{
	//No state
	_currentState = BLANK;
	_width = 16;
	_height = 2; 
	this->begin(_width, _height);
}
//PUBLIC
void MainDisplay::test(){


}

void MainDisplay::onScreen(){
	
	this->clear();
	this->printCenterUp("GLOBETROTTER");
	_currentState = ON;
	
	
}
void MainDisplay::readyScreen(int batteryStatus){
		this->clear();
		this->printLeftUp("Ready");
		this->printRigthUp((String)batteryStatus + "%");
		_currentState = READY;

}
void MainDisplay::receivingDataScreen(){

		this->clear();
		this->printCenterUp("GLOBETROTTER");
		this->printCenterDown("Receiving data");
		_currentState = RECEIVINGDATA;

}
void MainDisplay::startScreen(int timeToStart){
	this->clear();
	if(timeToStart >= 10){
		this->printCenterUp((String)timeToStart+"s to Start");
	}else{
		String t = (String)timeToStart+"s to Start";
		this->setCursor((int)((_width -  t.length()) / 2 +1), 0);
		this->print(t);
	}
	_currentState = START;
}
void MainDisplay::runningScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){

	this->clear();
	this->printLeftUp("Running");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_currentState = RUNNING;
	
}
void MainDisplay::pauseScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){

	this->clear();
	this->printLeftUp("Running");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_currentState = RUNNING;
	
}
void MainDisplay::doneScreen(int picture, int lastPicture, int batteryStatus, int timeLeft){

	this->clear();
	this->printLeftUp("Done");
	this->printLeftDown((String)picture + "/" + (String)lastPicture);

	this->printRigthUp((String)batteryStatus+"%");
	this->printRigthDown((String)timeLeft+"s");
	_currentState = RUNNING;
	
}
void MainDisplay::resettingScreen(){
	this->clear();
	this->printCenterUp("GLOBETROTTER");
	this->printCenterDown("Resetting");
}
void MainDisplay::resumeScreen(int timeToResume){
	this->clear();
	if(timeToResume >= 10){
		this->printCenterUp((String)timeToResume+"s to Resume");
	}else{
		String t = (String)timeToResume+"s to Resume";
		this->setCursor((int)((_width -  t.length()) / 2 +1), 0);
		this->print(t);
	}
	_currentState = START;
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
