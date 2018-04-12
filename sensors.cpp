#include "BrickPi3.h" /// For BrickPi3, this file sets up the BrickPi for usage
#include <iostream> /// For cout
#include <unistd.h> /// For sleep
#include <signal.h> /// For exit signals
#include <iomanip> /// For setw and setprecision
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

/// Setting up sensors as functions so they can be reused

/// Setup ultrasonic reading
int ultrasonic(){
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	sensor_ultrasonic_t Ultrasonic;
	if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
			cout << "Ultrasonic sensor sees this distance in cm: ";
	}
	return Ultrasonic.cm;
}

/// Setup left sensor reading
int leftBW(){
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	sensor_light_t Blacknwhitelinks;
	if(BP.get_sensor(PORT_1, Blacknwhitelinks) == 0){
		cout << "Left sees " << Blacknwhitelinks.reflected << endl;
	}
	else{
		cout << "Left sees black" << endl;
	}
	return Blacknwhitelinks.reflected;
}

/// Setup right sensor reading
int rightBW(){
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_LIGHT_ON);
	sensor_light_t Blacknwhiterechts;
	if(BP.get_sensor(PORT_2, Blacknwhiterechts) == 0){
		cout << "Right sees " << Blacknwhiterechts.reflected << endl;
	}
	else{
		cout << "Right sees black" << endl;
	}
	return BlacknWhiterechts.reflected;
}

/// Main function is used to check all sensors, f.e. for testing
int main(){
	signal(SIGINT, exit_signal_handler); /// Exit function for ctrl c
	
	BP.detect(); /// Make sure Pi is communicating and up to date
	
	int error;
	
	while (true){ /// Check sensors
		error = 0;
		ultrasonic();
		linksZW();
		rechtsZW();
	}
}

/// Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}
