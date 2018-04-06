#include "BrickPi3.h" //for BrickPi3, this file sets up the BrickPi for usage
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
#include <vector>
using namespace std;

BrickPi3 BP;

// give portnumbers
uint8_t motorleft = PORT_C;
uint8_t motorright = PORT_B;
uint16_t BWsensorleft = PORT_2;
uint16_t BWsensorright = PORT_1;
uint8_t motortop = PORT_D;
uint8_t ultrasonic = PORT_3;

void exit_signal_handler(int signo);

void movement(uint8_t powerleft, uint8_t powerright){
    // Move Arnold forward
    BP.set_motor_power(motorleft, powerleft);
    BP.set_motor_power(motorright, powerright);
}

vector <uint16_t> calibrar(){
    string ready;
    cout << "Place linebot on position with straight line (make sure both Black/White sensors are on white)" << endl;
    cout << "Are you ready (yes?): " << endl;
    cin >> ready;
    if(ready != "yes"){
        calibrar();
    }
    BP.getsensor(BWsensorleft, left);
    BP.getsensor(BWsensorright, right);
    vector<int> final;
    uint16_t leftwhite1 = left.reflected;
    uint16_t rightwhite1 = right.reflected;
    uint16_t leftblack1 = left.reflected;
    uin16_t rightblack1 = right.reflected;



    //measure Black white sensor on spot one
    movement(50,50);
    usleep(2000000);
    movement(50,0);
    if(left.reflected > (leftwhite+400)){
        movement(0,0);
        uint16_t leftblack1 = left.reflected;
    }
    movement(0,50);
    if(right.reflected > (rightwhite+400)) {
        movement(0, 0);
        uint16_t rightblack1 = right.reflected;
    }
    movement(50,0);
    if(left.reflected < 1900 && right.reflected < 1900){
        movement(0,0);
    }
    //measure Black white sensors on spot two
    uint16_t leftwhite2 = left.reflected;
    uint16_t rightwhite2 = right.reflected;
    uint16_t leftblack2 = left.reflected;
    uint16_t rightblack2 = right.reflected;
    movement(50,50);
    usleep(2000000);
    movement(50,0);
    if(left.reflected > (leftwhite+400)){
        movement(0,0);
        uint16_t leftblack2 = left.reflected;
    }
    movement(0,50);
    if(right.reflected > (rightwhite+400)) {
        movement(0, 0);
        uint16_t rightblack2 = right.reflected;
    }
    movement(50,0);
    if(left.reflected < 1900 && right.reflected < 1900){
        movement(0,0);
    }
    movement(-50,-50);
    sleep(4000000);
    movement(0,0);
    uint16_t leftwhite = (leftwhite1 + leftwhite2)/2;
    uint16_t rightwhite = (rightwhite1 + rightwhite2)/2;
    uint16_t leftblack = (leftblack1 + leftblack2)/2;
    uint16_t rightblack = (rightblack1 + rightblack2)/2;
    vector<uint16_t> final (leftwhite ,rightwhite, leftblack, rightblack);
    return final;
}

int main(){
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;
    	string go;
    
    //setting up sensors
	//BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	
	sensor_light_t bwleft;
	sensor_light_t bwright;
    
    vector<uint16_t> bwvalues = calibrar();
    uint16_t whiteLeft = bwvalues[0];
    uint16_t whiteRight = bwvalues[1];
    uint16_t blackLeft = bwvalues[2];
    uint16_t blackLeft = bwvalues[3];

    cout << "White left:" << whiteLeft << endl;
    cout << "White right:" << whiteRight << endl;
    cout << "Black left:" << blackLeft << endl;
    cout << "Black Right:" << blackRight << endl;
	
    cout << "If you're ready press 'y' followed by enter" << endl;
    cin >> go;
    while(go != "y"){
    cout << "If you're ready press 'y' followed by enter" << endl;
    cin >> go;
    }
	
	while (true){
		error = 0;
        
        //if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
		//	cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
		//}
		if(BP.get_sensor(BWsensorright, bwright) == 0){
			cout << "Right sees " << bwright.reflected << endl;
            if(bwright.reflected > 2100){
                //set right motor speed to lower value
                movement(50,-50);
            }
            else{
                //set right motor speed to normal value
                movement(50,50);
            }
		}
        else{
            cout << "Right light sensor not working!" << endl;
        }
		if(BP.get_sensor(BWsensorleft, bwleft) == 0){
			cout << "Left sees " << bwleft.reflected << endl;
            if(bwleft.reflected > 2100){
                //set left motor speed to lower value
                movement(-50,50);
            }
            else{
                //set left motor speed to normal value
                movement(50,50);
            }
		}
		else{
            cout << "Left light sensor not working!" << endl;
        }
    }
}

//Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

