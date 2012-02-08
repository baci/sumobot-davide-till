#include "pilot.h"
#include "servos.h"
#include "LED.h"
#include "timer.h"

void moveForward(int speed)
{
	clearGreen();
	clearRed();
	setMotorSpeed(speed, speed);
}
void moveBackward(int speed)
{
	setMotorSpeed(-speed, -speed);
}
void turnLeft(int speed)
{
	setMotorSpeed(speed, 0);
}
void turnBackLeft(int speed)
{
	setMotorSpeed(0, -speed);
	setGreen();
	delay(700);
}
void spinLeft(int speed)
{
	setMotorSpeed(speed, -speed);
}
void turnRight(int speed)
{
	setMotorSpeed(0, speed);
}
void turnBackRight(int speed)
{
	setMotorSpeed(-speed, 0);
	setRed();
	delay(700);
}
void spinRight(int speed)
{
	setMotorSpeed(-speed, speed);
}
