#include "BrickPi3.h"
#include "Arnold.h" 
#include <signal.h> //for exiting
#include <iostream> //for cout and cin
#include <iomanip> //for setw and setprecision
#include <unistd.h> //for sleep

BrickPi3 BP;
void exit_signal_handler(int signo);

Arnold::Arnold() {
	signal(SIGINT, exit_signal_handler);
	BP.detect();
	BP.set_sensor_type(Arnold::BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(Arnold::BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(Arnold::ultrasonic, SENSOR_TYPE_NXT_ULTRASONIC);
}

int Arnold::getUltrasonic() {
	sensor_ultrasonic_t Ultrasonic;
	if(BP.get_sensor(Arnold::ultrasonic, Ultrasonic) == 0){
			cout << "Ultrasonic sensor sees this distance in cm: ";
		return Ultrasonic.cm;
	}
	return -1;
}

int Arnold::getLeftBW() {
	sensor_light_t Blacknwhitelinks;
	if(BP.get_sensor(Arnold::BWsensorleft, Blacknwhitelinks) == 0){
		cout << "Left sees " << Blacknwhitelinks.reflected << endl;
		return Blacknwhitelinks.reflected;
	}
	else{
		cout << "Left sees black" << endl;
	}
	return -1;
}

int Arnold::getRightBW() {
	sensor_light_t Blacknwhiterechts;
	if(BP.get_sensor(Arnold::BWsensorright, Blacknwhiterechts) == 0){
		cout << "Right sees " << Blacknwhiterechts.reflected << endl;
		return BlacknWhiterechts.reflected;
	}
	else{
		cout << "Right sees black" << endl;
	}
	return -1;
}

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-1);
	}
}
