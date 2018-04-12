#include "BrickPi3.h"
#include "Arnold.h" 
#include <signal.h> //for exiting
#include <iostream> //for cout and cin
#include <iomanip> //for setw and setprecision
#include <unistd.h> //for sleep

using namespace std;

BrickPi3 BP;
void exit_signal_handler(int signo);


///
/// Constructor Arnold class
///
/// This constructor will initialize the sensor and motor ports, init the BrickPi3 and set the sensor types
///
Arnold::Arnold() {
	signal(SIGINT, exit_signal_handler);
	BP.detect();
	
	motorleft = PORT_C;
	motorright = PORT_B;
	BWsensorleft = PORT_2;
	BWsensorright = PORT_1;
	motortop = PORT_D;
	ultrasonic = PORT_3;
	
	BP.set_sensor_type(Arnold::BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(Arnold::BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(Arnold::ultrasonic, SENSOR_TYPE_NXT_ULTRASONIC);
}

///
/// getUltrasonic()
///
/// Returns the distance in centimeters that the ultrasonic sensor measures
///
int Arnold::getUltrasonic() {
	sensor_ultrasonic_t Ultrasonic;
	BP.get_sensor(Arnold::ultrasonic, Ultrasonic);
	cout << Ultrasonic.cm << " cm" <<endl;
	return Ultrasonic.cm;
}

///
/// move(uint16_t powerLeft, uint16_t powerRight)
/// 	powerLeft: speed the left motor has to rotate
/// 	powerRight: speed the right motor has to rotate
///
/// this function will move arnolds base motors with the given speed
///
void Arnold::move(uint16_t powerLeft, uint16_t powerRight) {
	BP.set_motor_power(Arnold::motorleft, powerLeft);
	BP.set_motor_power(Arnold::motorright, powerRight);
}

///
/// stop()
///
/// this function will stop the motors to ensure no runaway motors.
///
void Arnold::stop()
{
	 BP.set_motor_power(Arnold::motorleft, 0);
	 BP.set_motor_power(Arnold::motorright, 0);
}

///
/// turn_ultrasonic(int position);
///		position: tells the function to turn the ultrasonic to the left or right with 90 degrees
/// 
/// turns the head to the left is position is 1, and to the right if position is 2
///
void Arnold::turn_ultrasonic(int position){
	if(position ==1){
		BP.set_motor_dps(Arnold::motortop ,90);
		sleep(1);
		BP.set_motor_dps(Arnold::motortop ,0);
		
	}
	else if(position == 2){
		BP.set_motor_dps(Arnold::motortop ,-90);
		sleep(1);
		BP.set_motor_dps(Arnold::motortop ,0);
	}	
}

///
/// getLeftBW()
/// 
/// Returns the reflection value the left reflection sensor measures
/// 
int Arnold::getLeftBW() {
	sensor_light_t Blacknwhitelinks;
	BP.get_sensor(Arnold::BWsensorleft, Blacknwhitelinks);
	return Blacknwhitelinks.reflected;
}

///
/// getRightBW()
/// 
/// Returns the reflection value the right reflection sensor measures
///
int Arnold::getRightBW() {
	sensor_light_t Blacknwhiterechts;
	BP.get_sensor(Arnold::BWsensorright, Blacknwhiterechts);
	return Blacknwhiterechts.reflected;
}

///
/// getLeftWhiteValue()
/// 
/// gets left white value that was set during calibration
///
int Arnold::getLeftWhiteValue() {
	return leftWhiteValue;
}

///
/// getRightWhiteValue()
/// 
/// gets right white value that was set during calibration
///
int Arnold::getRightWhiteValue() {
	return rightWhiteValue;
}

///
/// calibrate()
/// 
/// This function calibrates arnold by measuring the reflection on the left and right sensor on two spots. The average of the measurements is stored int the leftWhiteValue and rightWhiteValue
/// 
void Arnold::calibrate() {
	string ready;
    cout << "Place linebot on position with straight line (make sure both Black/White sensors are on white)" << endl;
    cout << "Are you ready (yes?): " << endl;
    cin >> ready;
    if(ready != "yes"){
        this -> calibrate();
    }
	
    uint16_t leftwhite1 = this->getLeftBW();
    uint16_t rightwhite1 = this->getRightBW();
	
	this->move(30,30);
	usleep(1000000);
	this->stop();
	
	uint16_t leftwhite2 = this->getLeftBW();
	uint16_t rightwhite2 = this->getRightBW();
	
	this->leftWhiteValue = (leftwhite1 + leftwhite2) / 2;
	this->rightWhiteValue = (rightwhite1 + rightwhite2) / 2;
	
	cout << "White left: " << this->getLeftWhiteValue() << endl;
	cout << "White right: " << this->getRightWhiteValue() << endl;
	
}

///
/// leftSideOnLine()
///
/// returns if the left side sensor is measuring something else than white
///
bool Arnold::leftSideOnLine() {
	uint16_t currentValueLeft = this->getLeftBW();
	if (currentValueLeft - this->BWMargin > this->getLeftWhiteValue() || currentValueLeft + this->BWMargin < this->getLeftWhiteValue()) {
		return true;
	} 
	return false;
}

///
/// rightSideOnLine()
///
/// returns if the right side sensor is measuring something else than white
///
bool Arnold::rightSideOnLine() {
	uint16_t currentValueRight = this->getRightBW();
	if (currentValueRight - this->BWMargin > this->getRightWhiteValue() || currentValueRight + this->BWMargin < this->getRightWhiteValue()) {
		return true;
	} 
	return false;
}

///
/// setBWMargin(uint16_t margin)
/// 	margin: the new margin
///
/// sets the margin that will be used in depending if the sensor is measuring white or not. Can be seen as a sensitivity setting
///
int Arnold::setBWMargin(uint16_t margin) {
	this->BWMargin = margin;
}

///
/// lineFollowAlgoritm()
///
/// This algoritm will follow the line by incrementing and decrementing the speed of the motors. 
///
void Arnold::lineFollowAlgoritm() {
	bool leftSideOnLine = this->leftSideOnLine();
	bool rightSideOnLine = this->rightSideOnLine();
		
	if (leftSideOnLine && rightSideOnLine) {
		usleep(55000);
	}
	
	usleep(1000);
	
	if (leftSideOnLine) {
		if (this->currentLeftPower > this->minLineFollowPower) {
			this->currentLeftPower--;
		}
	} else {
		if (this->currentLeftPower < this->maxLineFollowPower) {
			this->currentLeftPower++;
		}
	}
	
	if (rightSideOnLine) {
		if (this->currentRightPower > this->minLineFollowPower) {
			this->currentRightPower--;
		}
	} else {
		if (this->currentRightPower < this->maxLineFollowPower) {
			this->currentRightPower++;
		}
	}
	
	this->move(this->currentLeftPower, this->currentRightPower);
		
}

///
/// crossNavigator(int direction)
///		direction: the direction arnold has to travel on calling this function. 0 for left, 1 for straight, 2 for right turn, 3 for 180 turn
///
///	will move arnold at a crosspoint. This function doesn't detect crosspoints, only moves arnold when called. Normal usage is when a crossing is detected, this function is called to direct arnold where to go
///
void Arnold::crossNavigator(int direction) {
	if (direction == 0) { ///turn left
		this->move(40, 40);
		usleep(800000);
		this->stop();
		this->turn(1);
		return;
	}
	if (direction == 1) { ///go straight ahead
		this->move(40, 40);
		usleep(500000);
		this->stop();
		return;
	}
	if (direction == 2) { /// go right
		this->move(40, 40);
		usleep(800000);
		this->stop();
		this->turn(2);
		return;
	}
	if (direction == 3) { /// turn around
		this->turn(1);
		this->turn(1);
	}
}

///
/// goAroundObstacle()
///
/// passes a obstacle based on ultrasonic, always passes right
///
void Arnold::goAroundObstacle() {
    bool cont = true;
    this->turnDegrees(120);
    this->turn_ultrasonic(1);
    this->move(20,20);
    while(cont){
        if(getUltrasonic() > 15) {
            usleep(4000000);///After first turn drive 4 seconds after ultrasonic stops seeing object
            this->stop();
            cont=false;
            }
    }
    this->turn(1);
    this->move(20,20);
    usleep(4000000);///After second turn drive 4 seconds after ultrasonic stops seeing object	
    cont = true;
    while(cont){
        if(this->getUltrasonic() > 10) {
            usleep(4000000);
            this->stop();
            cont= false;
        }
    }
    this->turn(1);
    this->turn_ultrasonic(2);	
    this->move(20,20);
    while(true){ ///Make sure Arnold gets back on line
        if(this->getLeftBW()>1900 && this->getRightBW() >1900){
            this->stop();
            sleep(1);
            this->crossNavigator(2);
            break;
            }
    }
}


///
/// turn(int side)
///		side: indicates which side arnold will turn to
///
/// this function turns arnold 90 degrees to the left(side = 1) or right(side = 2)
///
void Arnold::turn(int side){
    
    ///one is left
    if(side==1){
        move(-50, 50);
        usleep(1082430);
        stop();
    }
    ///two is right
    if(side==2){ 
        move(50, -50);
        usleep(1082430);
        stop();
    }
}

///
/// turnDegrees(int degrees)
///	degrees: amount of degrees that arnold will turn
///
/// This function will turn arnold by a given amount of degrees, positive for to the right, negative for to the left
///
void Arnold::turnDegrees(int degrees) {
	if (degrees < 0) {
		this->move(-50, 50);
		usleep(12027*(degrees * -1));
		this->stop();
		return;
	}
	this->move(50, -50);
	usleep(12027*degrees);
	this->stop();
}

///
/// reset()
///
/// This function will stop the motors and reset the BrickPi
///
void Arnold::reset() {
	this->stop();
	BP.reset_all();
}

/// 
/// exit_signal_handler(int signo)
///
/// exit handler (ctrl-c)
///
void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-1);
	}
}





