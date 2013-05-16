#ifndef PanoramaSettings_h
#define PanoramaSettings_h


#include "Arduino.h"


class PanoramaSettings
{
  public:

  	//CONSTRUCT
    PanoramaSettings(float focal, float sensorH, float sensorV, float overlap);
    int getNrOfPictures();



    void 	setFocalLength(float focal);

    int		getPlusMinusEv();
	void 	setplusMinusEv(int ev);

	int		getMiddShutterSpeed();
	void 	setMiddShutterSpeed(int code);

    float 	calcPanCordinate(int picNr);
    float 	calcTiltCordinate(int picNr);

    int getShutterSpeed(int evNr);
  private:
  	float 	_focalLength; 
	float 	_sensorHorizontal; 
	float 	_sensorVertical; 
	float 	_overlap;

	int		_plusMinusEv;
	int		_midShutterSpeed;
	int		_midShutterSpeedIndex;

	int	  	_totalNrOfPictures;
	int	 	_horizontalPics;
	float	_verticalAngle;
	int		_verticalPics;
	int		_shutterSpeeds[55];
	void calcNrOfPictures();
	int getShutterSpeedIndex(int code);
   
};


#endif