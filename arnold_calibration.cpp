#include "BrickPi3.h" // For BrickPi3, this file sets up the BrickPi for usage
#include <iostream> // For cout
#include <unistd.h> // For sleep
#include <signal.h> // For exit signals
#include <iomanip> // For setw and setprecision
using namespace std;

BrickPi3 BP;

/// Gives portnumbers
uint8_t motorleft = PORT_C;
uint8_t motorright = PORT_B;
uint16_t BWsensorleft = PORT_2;
uint16_t BWsensorright = PORT_1;
uint8_t motortop = PORT_D;
uint8_t ultrasonic = PORT_3;

void exit_signal_handler(int signo);

void movement(uint8_t powerleft, uint8_t powerright){
    /// Moves Arnold forward
    BP.set_motor_power(motorleft, powerleft);
    BP.set_motor_power(motorright, powerright);
}

vector <int> calibrate() {
    string ready;
    cout << "Place Arnold in the right position on a straight line (make sure both lightsensors are on white)" << endl;
    cout << "Are you ready (yes)?: " << endl;
    cin >> ready;
    if (ready != "yes") { /// Make sure robot is in position
        calibrate();
    }

    int leftW;
    int rightW;
    int leftB;
    int rightB;

    int leftW2;
    int rightW2;
    int leftB2;
    int rightB2;

    int leftW3;
    int leftB3;
    int rightB3;
    int rightW3;


    vector<int> values;
	
	/// Calibrate white
	/** Puts both values in variable, then moves robot to next spot to calibrate line values*/
    cout << "White left lightlevel in spot 1 : " << leftBW() << endl;
    leftW = bwleft.reflected;
    cout << "White right lightlevel in spot 1 :  " << rightBW() << endl;
    rightW = bwright.reflected;

    movement(50, 50);
    usleep(1000000);
    movement(50, 0);
	
	/// Calibrate black
	/** Makes sure line values are higher than white values by a set margain. 
	 * This to make sure the robot is clear what the line is. Robot then moves to check other side */
    if(leftBW() > (leftW+200)){
        leftB = leftBW();
        movement(0,50);
        cout << "Black right lightlevel in spot 1 :  " << leftB << endl;
    }
    if(rightBW() > (rightW+200)){
        rightB = rightBW();
        movement(0,50);
        cout << "Black right lightlevel in spot 1 :  " << rightB << endl;
    }

	/// Make sure first values are set well
    if(leftBW() < (leftB-100) && rightBW() < (rightB-100)){
        movement(50,50);
        usleep(2000000);
    }

   /// Test values of lightsensors on second spot
   /** Repeat what happened in first test to make sure all values are calibrated well with a second test. 
    * End result is the middle of 2 test values to ensure trustability*/
	
	/// White lightlevel values
    cout << "White left lightlevel in spot 1 : " << leftBW() << endl;
    leftW2 = bwleft.reflected;
    cout << "White right lightlevel in spot 1 :  " << rightBW() << endl;
    rightW2 = bwright.reflected;
	
    movement(50, 0);
	
	/// Black lightlevel values
    if(leftBW() > (leftW2+200)){
        leftB2 = leftBW();
        movement(0,50);
        cout << "Black right lightlevel in spot 1 :  " << leftB2 << endl;
    }
    if(rightBW() > (rightW2+200)){
        rightB2 = rightBW();
        movement(0,50);
        cout << "Black right lightlevel in spot 1 :  " << rightB2 << endl;
    }
    
	/// Getting average of the 2 spot measurements
    leftW3 = (leftW+leftW2)/2;
    leftB3 = (leftB+leftB2)/2;
    rightW3 = (rightW+rightW2)/2;
    rightB3 = (rightB+rightB2)/2;

    values = {leftW3, leftB3, rightW3, rightB3};
    return values;
}



/// Main is used in testing to ensure sensors are well set.
int main(){
	signal(SIGINT, exit_signal_handler); /// Exit function for ctrl c
	
	BP.detect(); /// Make sure Pi is communicating and up to date
	
	int error;
    string cont;
    
    /// Setting up sensors
	BP.set_sensor_type(BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	
	sensor_light_t bwleft;
	sensor_light_t bwright;
	
    /// Using measured values and giving them 200 margin to ensure trustability
    vector<int> calibrateValues = calibrate();
    
    int leftWhite = calibrateValues[0]+200;
    int leftBlack = calibrateValues[1]+200;
	int rightWhite = calibrateValues[2]-200;
    int rightBlack = calibrateValues[3]-200;
    
    /// Checking if Arnold is in right position to start
    cout << "Left lightsensor values are: " << calibrateValues[0] << "for white and " <<calibrateValues[1] << "for black." << endl;
    cout << "Right lightsensor values are: " << calibrateValues[2] << "for white and " <<calibrateValues[3] << "for black." << endl;
    cout << "Are you on starting spot? (yes)?" << endl;
    cin >> cont;
    while(cont != "yes"){
    cout << endl;
    cout << "Are you on starting spot? (yes?)" << endl;
    cin >> cont;
    }
}


///Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

