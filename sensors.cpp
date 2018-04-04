#include "BrickPi3.h" //for BrickPi3, this file sets up the BrickPi for usage
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
using namespace std;

void exit_signal_handler(int signo);

int main(){
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;
	
	//setting up sensors
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4 SENSOR_TYPE_TOUCH);
	
	sensor_ultrasonic_t Ultrasonic;
	sensor_touch_t Touch;
	sensor_light_t Blacknwhitelinks;
	sensor_light_t Blacknwhiterechts;
	
	while (true){
		error = 0;
		
		if (BP.get_sensor(PORT_3, Ultrasonic) == 0){
			cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
		}
		if(BP.get_sensor(PORT_4, Touch) == 0){
			cout << "Touch sensor was pressed" << endl;
		}
		if(BP.get_sensor(PORT_1, Blacknwhitelinks) > 2400){
			cout << "Left sees white" << endl;
		}
		else{
			cout << "Left sees black" << endl;
		}
		if(BP.get_sensor(PORT_2, Blacknwhiterechts) > 2400){
			cout << "Right sees white" << endl;
		}
		else{
			cout << "Right sees black" << endl;
		}
		
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