#ifndef Bluetooth_h
#define Bluetooth_h


class Bluetooth
{
public:
	Bluetooth(int blueLedPin);

	bool newDataToRead();
	void updated();
	//Types of updates
	bool updateCameraSetting();
	bool updateEvVariable();
	bool updateFocalLength();
	bool updateShutterVariable();
	bool startStopProgram();
	bool shouldTakePicture();
	bool moveCamera();
	bool shouldStart();
	void handleAction();
	//READ 
	int readPropertyValue();
	int readPropertyName();
	

	//Bt Run motors
	bool moveCameraUp();
	bool moveCameraDown();
	bool moveCameraLeft();
	bool moveCameraRight();
	bool moveCameraHome();
	bool moveCameraStop();
	bool anyMotorRunning();
private:
	int 	_blueLedPin;
	char	_action;
	float  	_propertyValue;
	int		_propertyName;



	bool	_updateCameraSettings;
	bool	_updateEvVariable;
	bool	_updateShutterVariable;
	bool	_updateFocalVariable;
	bool 	_shouldStart;
	bool	_shouldMoveCamera;
	bool	_shouldTakePicture;
	bool	_manualMoveCameraUp;
	bool	_manualMoveCameraDown;
	bool	_manualMoveCameraLeft;
	bool	_manualMoveCameraRight;
	bool	_manualMoveCameraHome;
	bool	_manualMoveCameraStop;

	void handleStatusLed();
	float readFloatSerial();
	/* data */
};

#endif