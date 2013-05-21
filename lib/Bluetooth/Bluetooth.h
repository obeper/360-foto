#ifndef Bluetooth_h
#define Bluetooth_h value


class Bluetooth
{
public:
	Bluetooth(int blueLedPin);

	bool newDataToRead();
	void updated();
	//Types of updates
	bool updateCameraSetting();
	bool updateEvVariable();
	bool updateShutterVariable();
	bool startStopProgram();
	bool shouldTakePicture();
	bool moveCamera();

	void handleAction();
	//LÄSA
	int readPropertyValue();
	int readPropertyName();
	bool shouldStart();
	
private:
	int 	_blueLedPin;
	char	_action;
	float  	_propertyValue;
	int		_propertyName;



	bool	_updateCameraSettings;
	bool	_updateEvVariable;
	bool	_updateShutterVariable;
	bool 	_shouldStart;
	bool	_shouldMoveCamera;
	bool	_shouldTakePicture;

	void handleStatusLed();
	float readFloatSerial();
	/* data */
};

#endif