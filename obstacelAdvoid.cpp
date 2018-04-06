#include "BrickPi3.h"

BrickPi3 BP;

void exit_signal_handler(int signo);


int main() {
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;

	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	
	sensor_ultrasonic_t Ultrasonic;
	
	if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
			if (Ultrasonic.cm < 15) {
				//OBSTACLE DETECTED
			} else {
				//NO OBSTACLE DETECTED
			}
	}
	
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}