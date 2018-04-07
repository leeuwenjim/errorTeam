#ifndef ARNOLDCLASS
#define ARNOLDCLASS

#include "BrickPi3.h"

class Arnold {
private:
	uint8_t motorleft = PORT_C;
	uint8_t motorright = PORT_B;
	uint16_t BWsensorleft = PORT_2;
	uint16_t BWsensorright = PORT_1;
	uint8_t motortop = PORT_D;
	uint8_t ultrasonic = PORT_3;
public:
	Arnold();
	void move(uint8_t powerLeft, uint8_t powerRight);
	int getUltrasonic();
	int getLeftBW();
	int getRightBW();
}

#endif
