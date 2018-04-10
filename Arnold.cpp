#include "BrickPi3.h"
#include "Arnold.h" 
#include <signal.h> //for exiting
#include <iostream> //for cout and cin
#include <iomanip> //for setw and setprecision
#include <unistd.h> //for sleep

using namespace std;

BrickPi3 BP;
void exit_signal_handler(int signo);



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

int Arnold::getUltrasonic() {
	sensor_ultrasonic_t Ultrasonic;
	BP.get_sensor(Arnold::ultrasonic, Ultrasonic);
	//if(BP.get_sensor(Arnold::ultrasonic, Ultrasonic) == 0){
	//		cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
	//}
	return Ultrasonic.cm;
}

void Arnold::move(uint16_t powerLeft, uint16_t powerRight) {
    //BP.set_motor_power(Arnold::motorleft, powerLeft);
    //BP.set_motor_power(Arnold::motorright, powerRight);
	
	BP.set_motor_power(Arnold::motorleft, powerLeft);
	BP.set_motor_power(Arnold::motorright, powerRight);
	//this->currentLeftPower = powerLeft;
	//this->currentRightPower = powerRight;
}

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

void Arnold::stop(void)
{
	 BP.set_motor_power(Arnold::motorleft, 0);
	 BP.set_motor_power(Arnold::motorright, 0);
	 //this->currentLeftPower = 0;
	 //this->currentRightPower = 0;
}

int Arnold::getLeftBW() {
	sensor_light_t Blacknwhitelinks;
	if(BP.get_sensor(Arnold::BWsensorleft, Blacknwhitelinks) == 0){
		//cout << "Left sees " << Blacknwhitelinks.reflected << endl;
	}
	else{
		//cout << "Left sees black" << endl;
	}
	return Blacknwhitelinks.reflected;
}

int Arnold::getRightBW() {
	sensor_light_t Blacknwhiterechts;
	if(BP.get_sensor(Arnold::BWsensorright, Blacknwhiterechts) == 0){
		//cout << "Right sees " << Blacknwhiterechts.reflected << endl;
	}
	else{
		//cout << "Right sees black" << endl;
	}
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

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-1);
	}
}


