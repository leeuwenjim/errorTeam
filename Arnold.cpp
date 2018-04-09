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
	if(BP.get_sensor(Arnold::ultrasonic, Ultrasonic) == 0){
			cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
	}
	return Ultrasonic.cm;
}

void Arnold::move(uint16_t powerLeft, uint16_t powerRight) {
    //BP.set_motor_power(Arnold::motorleft, powerLeft);
    //BP.set_motor_power(Arnold::motorright, powerRight);
	
	BP.set_motor_power(Arnold::motorleft, powerLeft);
	BP.set_motor_power(Arnold::motorright, powerRight);
}

void Arnold::turn_ultrasonic(int position){
	if(position ==1){
		BP.set_motor_position(Arnold::motortop,0)
	}
	elif(position ==2){
		BP.set_motor_position(Arnold::motortop ,90)
	}
	elif(position == 3){
		BP.set_motor_position(Arnold::motortop ,180)
	}
}

void Arnold::stop(void)
{
	 BP.set_motor_power(Arnold::motorleft, 0);
	 BP.set_motor_power(Arnold::motorright, 0);
}

int Arnold::getLeftBW() {
	sensor_light_t Blacknwhitelinks;
	if(BP.get_sensor(Arnold::BWsensorleft, Blacknwhitelinks) == 0){
		cout << "Left sees " << Blacknwhitelinks.reflected << endl;
	}
	else{
		cout << "Left sees black" << endl;
	}
	return Blacknwhitelinks.reflected;
}

int Arnold::getRightBW() {
	sensor_light_t Blacknwhiterechts;
	if(BP.get_sensor(Arnold::BWsensorright, Blacknwhiterechts) == 0){
		cout << "Right sees " << Blacknwhiterechts.reflected << endl;
	}
	else{
		cout << "Right sees black" << endl;
	}
	return Blacknwhiterechts.reflected;
}

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-1);
	}
}


