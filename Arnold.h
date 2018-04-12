#ifndef ARNOLDCLASS
#define ARNOLDCLASS

#include "BrickPi3.h"

class Arnold {
private:
	uint8_t motorleft; ///< left motor port
	uint8_t motorright; ///< right motor port
	uint16_t BWsensorleft; ///< left reflection sensor port
	uint16_t BWsensorright; ///< right reflection sensor port
	uint8_t motortop; ///< ultrasonic motor port
	uint8_t ultrasonic; ///< ultrasonic sensor port
	int leftWhiteValue; ///< calibrated white value on the left side
	int rightWhiteValue; ///< calibrated white value on the right side
	int BWMargin = 100; ///< margin used during line following, can be seen as sensitivity
	int maxLineFollowPower = 40; ///< line follow maximum speed
	int minLineFollowPower = -15; ///< line follow minimum speed
	int currentLeftPower = 15; ///< power of the left motor during line following
	int currentRightPower = 15; ///< power of the right motor during line following
public:
	Arnold(); ///< constructor
	void move(uint16_t powerLeft, uint16_t powerRight); ///< set motor speeds
	void stop(); ///< stop motors
	void turn_ultrasonic(int position); ///< turn ultrasonic motor
	int getUltrasonic(); ///< get value from ultrasonic
	int getLeftBW(); ///< get value from left reflection sensor
	int getRightBW(); ///< get value from right reflection sensor
	int getLeftWhiteValue(); ///< get white calibrated value on the left side
	int getRightWhiteValue(); ///< get white calibrated value on the right side
	void calibrate(); ///< calibrate arnold
	bool leftSideOnLine(); ///< check if left side is on the line
	bool rightSideOnLine(); ///< check if right side is on the line
	int setBWMargin(uint16_t margin); ///< set margin for line following
	void lineFollowAlgoritm(); ///< line following function, only a single iteration
	void crossNavigator(int direction); ///< turn left, right, goes ahead or turns around on a crossing (doesn't detect crossings)
	void turn(int side); ///< turns arnold 90 degrees
	void turnDegrees(int degrees);
	void goAroundObstacle(); ///< function that goes around an obstacle, and gets back on the line
	void reset(); ///< fuction that will reset arnold
};

#endif
