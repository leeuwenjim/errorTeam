#include "Arnold.h" //for using sensor functions
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <iomanip> //for setw and setprecision
#include <vector> //for using vectors
using namespace std;

Arnold arnold;

vector <int> calibrar() {
    
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

    usleep(1000000);	
    arnold.move(5, 5);
    usleep(1000000);
    arnold.move(5, 0);
    if(arnold.getLeftBW() > (leftW+200)){
        leftB = arnold.getLeftBW();
        arnold.move(0,5);
        cout << "Black right sees in spot 1 :  " << leftB << endl;
    }
    if(arnold.getRightBW() > (rightW+200)){
        rightB = arnold.getRightBW();
        arnold.move(0,5);
        cout << "Black right sees in spot 1 :  " << rightB << endl;
    }
    if(arnold.getLeftBW() < (leftB-100) && arnold.getRightBW() < (rightB-100)){
        arnold.move(5,5);
        usleep(2000000);
    }

   //test values of Black white sensors on second spot
    cout << "White left sees in spot 1 : " << arnold.getLeftBW() << endl;
    leftW2 = arnold.getLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.getRightBW() << endl;
    rightW2 = arnold.getRightBW();

    arnold.move(5, 0);
    if(arnold.getLeftBW() > (leftW2+200)){
        leftB2 = arnold.getLeftBW();
        arnold.move(0,5);
        cout << "Black right sees in spot 1 :  " << leftB2 << endl;
    }
    if(arnold.getRightBW() > (rightW2+200)){
        rightB2 = arnold.getRightBW();
        arnold.move(0,5);
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

