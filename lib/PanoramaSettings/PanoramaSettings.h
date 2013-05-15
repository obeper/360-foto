#ifndef PanoramaSettings_h
#define PanoramaSettings_h


#include "Arduino.h"


class PanoramaSettings
{
  public:

  	//CONSTRUCT
    PanoramaSettings(float focal, float sensorH, float sensorV, float overlap);
    int getNrOfPictures();
    void setFocalLength(float focal);
    float calcPanCordinate(int picNr);
    float calcTiltCordinate(int picNr);
  private:
  	float 	_focalLength; 
	float 	_sensorHorizontal; 
	float 	_sensorVertical; 
	float 	_overlap;

	int	  	_totalNrOfPictures;
	int	 	_horizontalPics;
	float	_verticalAngle;
	int		_verticalPics;
	void calcNrOfPictures();
	
   
};


#endif