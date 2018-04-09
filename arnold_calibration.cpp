#include "Arnold.h" //for using sensor functions
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
#include <vector> //for using vectors
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

vector <int> calibrar() {
    Arnold arnold;
    
    string ready;
    cout << "Place linebot on position with straight line (make sure both Black/White sensors are on white)" << endl;
    cout << "Are you ready (yes?): " << endl;
    cin >> ready;
    if (ready != "yes") {
        calibrar();
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

    cout << "White left sees in spot 1 : " << arnold.GetLeftBW() << endl;
    leftW = arnold.GetLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.GetRightBW() << endl;
    rightW = arnold.GetRightBW();

    movement(50, 50);
    usleep(1000000);
    movement(50, 0);
    if(arnold.GetLeftBW() > (leftW+200)){
        leftB = arnold.GetLeftBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << leftB << endl;
    }
    if(arnold.GetRightBW() > (rightW+200)){
        rightB = arnold.GetRightBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << rightB << endl;
    }
    if(arnold.GetLeftBW() < (leftB-100) && arnold.GetRightBW() < (rightB-100)){
        movement(50,50);
        usleep(2000000);
    }

   //test values of Black white sensors on second spot
    cout << "White left sees in spot 1 : " << arnold.GetLeftBW() << endl;
    leftW2 = arnold.GetLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.GetRightBW() << endl;
    rightW2 = arnold.GetRightBW();

    movement(50, 0);
    if(arnold.GetLeftBW() > (leftW2+200)){
        leftB2 = arnold.GetLeftBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << leftB2 << endl;
    }
    if(arnold.GetRightBW() > (rightW2+200)){
        rightB2 = arnold.GetRightBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << rightB2 << endl;
    }
    //getting average of the 2 spot measurements

    leftW3 = (leftW+leftW2)/2;
    leftB3 = (leftB+leftB2)/2;
    rightW3 = (rightW+rightW2)/2;
    rightB3 = (rightB+rightB2)/2;

    values = {leftW3, leftB3, rightW3, rightB3};
    return values;
}




int main(){
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;
    string cont;
    
    //setting up sensors
	//BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	
	sensor_light_t bwleft;
	sensor_light_t bwright;
    //using measert Black/White sensor values and giving them 200 margin
    vector<int> calibrateValues = calibrar();
    
    int leftWhite = calibrateValues[0]+200;
    int leftBlack = calibrateValues[1]+200;
	int rightWhite = calibrateValues[2]-200;
    int rightBlack = calibrateValues[3]-200;
    
    //checking if Arnold is on wright position to start
    cout << "Left Black/White sensor values are: " << calibrateValues[0] << "for white and: " <<calibrateValues[1] << "for black." << endl;
    cout << "Right Black/White sensor values are: " << calibrateValues[2] << "for white and: " <<calibrateValues[3] << "for black." << endl;
    cout << "Are you on starting spot? (yes?)" << endl;
    cin >> cont;
    while(cont != "yes"){
    cout << endl;
    cout << "Are you on starting spot? (yes?)" << endl;
    cin >> cont;
    }
    
    
    
	while (true){
		error = 0;
        
        //if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
		//	cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
		//}

        if(rightBW() > rightBlack){
                //set right motor speed to lower value
                movement(50,-50);
        }
        else{
                //set right motor speed to normal value
                movement(50,50);
        }
		if(leftBW() > leftBlack){
                //set left motor speed to lower value
                movement(-50,50);
        }
        else{
                //set left motor speed to normal value
                movement(50,50);
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

