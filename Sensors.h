#ifndef _SENSORS
#define _SENSORS

#define LEFT_LINE_POWER		PB4
#define RIGHT_LINE_POWER	PB2
#define LEFT_LINE_IN		PB7
#define RIGHT_LINE_IN		PB3

typedef struct _Emitter{
	int leftOut;
	int rightOut;
	int durationOut;
} Emitter;

typedef struct _Detector{
	int leftIn;
	int backIn;
	int rightIn;
	int durationIn;
} Detector;

typedef struct _LineSensor{
	int leftIn;
	int rightIn;
} LineSensor;

Emitter *emitter;
Detector *detector;
LineSensor *lineSensor;

int readLineSensor(int _sensor);
Emitter EmitterInit();
Detector DetectorInit();
LineSensor LineSensorInit();



#endif
