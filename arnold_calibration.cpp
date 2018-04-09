#include "Arnold.h" //for using sensor functions
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
#include <vector> //for using vectors
using namespace std;

BrickPi3 BP;


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

    cout << "White left sees in spot 1 : " << arnold.getLeftBW() << endl;
    leftW = arnold.getLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.getRightBW() << endl;
    rightW = arnold.getRightBW();

    movement(50, 50);
    usleep(1000000);
    movement(50, 0);
    if(arnold.getLeftBW() > (leftW+200)){
        leftB = arnold.getLeftBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << leftB << endl;
    }
    if(arnold.getRightBW() > (rightW+200)){
        rightB = arnold.getRightBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << rightB << endl;
    }
    if(arnold.getLeftBW() < (leftB-100) && arnold.getRightBW() < (rightB-100)){
        movement(50,50);
        usleep(2000000);
    }

   //test values of Black white sensors on second spot
    cout << "White left sees in spot 1 : " << arnold.getLeftBW() << endl;
    leftW2 = arnold.getLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.getRightBW() << endl;
    rightW2 = arnold.getRightBW();

    movement(50, 0);
    if(arnold.getLeftBW() > (leftW2+200)){
        leftB2 = arnold.getLeftBW();
        movement(0,50);
        cout << "Black right sees in spot 1 :  " << leftB2 << endl;
    }
    if(arnold.getRightBW() > (rightW2+200)){
        rightB2 = arnold.getRightBW();
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
    
    
    
 }



//Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

