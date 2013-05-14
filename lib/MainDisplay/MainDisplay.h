
#ifndef MainDisplay_h
#define MainDisplay_h


#include "Arduino.h"
#include "LiquidCrystal.h"


class MainDisplay : public LiquidCrystal
{
  public:

  	//CONSTRUCT
    MainDisplay(int rs, int enable,int d4, int d5, int d6, int d7);
    //SCREENS
    void onScreen();
    void readyScreen(int batteryStatus);
    void receivingDataScreen();
    void startScreen(int timeToStart);
    void runningScreen(int picture, int lastPicture, int batteryStatus, int timeLeft);
    void pauseScreen(int picture, int lastPicture, int batteryStatus, int timeLeft);
    void doneScreen(int picture, int lastPicture, int batteryStatus, int timeLeft);
    void resettingScreen();
    void resumeScreen(int timeToResume);
    void setRefreshRate(unsigned long refreshRate);

  private:
    int 			    _width;
    int 			    _height;
    unsigned long       _lastScreenUpdate; 
    unsigned long       _refreshRate;

    void printCenterUp(String wordToPrint);
    void printCenterDown(String wordToPrint);
    void printLeftUp(String wordToPrint);
    void printLeftDown(String wordToPrint);
    void printRigthUp(String wordToPrint);
    void printRigthDown(String wordToPrint);
    bool shouldRefresh();
};


#endif