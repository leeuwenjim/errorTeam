#include "BrickPi3.h" //for BrickPi3, this file sets up the BrickPi for usage
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

//Setting up sensors as functions so they can be reused
int ultrasonic(){
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	sensor_ultrasonic_t Ultrasonic;
	if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
			cout << "Ultrasonic sensor sees this distance in cm: ";
	}
	return Ultrasonic.cm;
}

int linksZW(){
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	sensor_light_t Blacknwhitelinks;
	if(BP.get_sensor(PORT_1, Blacknwhitelinks) == 0){
		cout << "Left sees " << Blacknwhitelinks.reflected << endl;
	}
	else{
		cout << "Left sees black" << endl;
	}
}

int rechtsZW(){
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_LIGHT_ON);
	sensor_light_t Blacknwhiterechts;
	if(BP.get_sensor(PORT_2, Blacknwhiterechts) == 0){
		cout << "Right sees " << Blacknwhiterechts.reflected << endl;
	}
	else{
		cout << "Right sees black" << endl;
	}
}

int main(){
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;
	
	while (true){ //check sensors
		error = 0;
		ultrasonic();
		linksZW();
		rechtsZW();
    		sleep(1); //wait before next check
	}
}

//Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}