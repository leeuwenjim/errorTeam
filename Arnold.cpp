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
/// This constructor will initialize the sensor and motor ports, init the BrickPi3 and set te sensor types
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
/// Returns the distance in centimeters that the ultrasonic sensor mesures
///
int Arnold::getUltrasonic() {
	sensor_ultrasonic_t Ultrasonic;
	BP.get_sensor(Arnold::ultrasonic, Ultrasonic);
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
/// this function will stop the motors on arnolds base.
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
/// turns the 
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

int Arnold::getLeftBW() {
	sensor_light_t Blacknwhitelinks;
	BP.get_sensor(Arnold::BWsensorleft, Blacknwhitelinks);
	return Blacknwhitelinks.reflected;
}

int Arnold::getRightBW() {
	sensor_light_t Blacknwhiterechts;
	BP.get_sensor(Arnold::BWsensorright, Blacknwhiterechts);
	return Blacknwhiterechts.reflected;
}


int Arnold::getLeftWhiteValue() {
	return leftWhiteValue;
}

int Arnold::getRightWhiteValue() {
	return rightWhiteValue;
}

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
	
    /// move forward for calibration
	this->move(30,30);
	usleep(1000000);
	this->stop();
	
	uint16_t leftwhite2 = this->getLeftBW();
	uint16_t rightwhite2 = this->getRightBW();
	
	this->leftWhiteValue = (leftwhite1 + leftwhite2) / 2;
	this->rightWhiteValue = (rightwhite1 + rightwhite2) / 2;
	
	cout << "White left: " << this->getLeftWhiteValue() << endl;
	cout << "White right: " << this->getRightWhiteValue() << endl;
	
    /// move backwards to start position
    this->move(-30,-30);
    usleep(1000000);
    this->stop();
}

bool Arnold::leftSideOnLine() {
	uint16_t currentValueLeft = this->getLeftBW();
	if (currentValueLeft - this->BWMargin > this->getLeftWhiteValue() || currentValueLeft + this->BWMargin < this->getLeftWhiteValue()) {
		return true;
	} 
	return false;
}

bool Arnold::rightSideOnLine() {
	uint16_t currentValueRight = this->getRightBW();
	if (currentValueRight - this->BWMargin > this->getRightWhiteValue() || currentValueRight + this->BWMargin < this->getRightWhiteValue()) {
		return true;
	} 
	return false;
}

int Arnold::setBWMargin(uint16_t margin) {
	this->BWMargin = margin;
}

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

void Arnold::crossNavigator(int direction) {
	if (direction == 0) { //turn left
		this->move(40, 40);
		usleep(800000);
		this->stop();
		this->turn(1);
		return;
	}
	if (direction == 1) { //go streaght ahead
		this->move(40, 40);
		usleep(500000);
		this->stop();
		return;
	}
	if (direction == 2) { // go right
		this->move(40, 40);
		usleep(800000);
		this->stop();
		this->turn(2);
		return;
	}
}

void Arnold::turn(int side){
    
    ///one is left
    if(side==1){
        move(-40, 40);
        usleep(1082430);
        stop();
    }
    ///two is right
    if(side==2){ 
        move(40, -40);
        usleep(1082430);
        stop();
    }
}

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-1);
	}
}


