#ifndef ARNOLDCLASS
#define ARNOLDCLASS

#include "BrickPi3.h"

class Arnold {
private:
	uint8_t motorleft;
	uint8_t motorright;
	uint16_t BWsensorleft;
	uint16_t BWsensorright;
	uint8_t motortop;
	uint8_t ultrasonic;
	int leftWhiteValue;
	int rightWhiteValue;
public:
	Arnold();
	void move(uint16_t powerLeft, uint16_t powerRight);
	void stop();
	void turn_ultrasonic(int position);
	int getUltrasonic();
	int getLeftBW();
	int getRightBW();
	int getLeftWhiteValue();
	int getRightWhiteValue();
	void calibrate();
};

#endif
