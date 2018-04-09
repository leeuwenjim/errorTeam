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

    int leftW = 0;
    int rightW =0;


    int leftW2 = 0;
    int rightW2 = 0;

    int leftW3 =0;
    int rightW3 = 0;


    vector<int> values;

    cout << "White left sees in spot 1 : " << arnold.getLeftBW() << endl;
    leftW = arnold.getLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.getRightBW() << endl;
    rightW = arnold.getRightBW();

    sleep(3);	
    arnold.move(20, 20);
    sleep(2);
    arnold.stop();
    sleep(2);
    arnold.move(20, 0);
  

   //test values of Black white sensors on second spot
    cout << "White left sees in spot 1 : " << arnold.getLeftBW() << endl;
    leftW2 = arnold.getLeftBW();
    cout << "White right sees in spot 1 :  " << arnold.getRightBW() << endl;
    rightW2 = arnold.getRightBW();

    

    leftW3 = (leftW+leftW2)/2;
    rightW3 = (rightW+rightW2)/2;

    values = {leftW3, rightW3 };
    return values;
}




int main(){
	
	int error;
    string cont;

    vector<int> calibrateValues = calibrar();
    
    int leftWhite = calibrateValues[0]+200;
    int rightWhite = calibrateValues[1]-200;
    
    //checking if Arnold is on wright position to start
    cout << "Left Black/White sensor values are: " << calibrateValues[0] << "for white" << endl;
    cout << "Right Black/White sensor values are: " << calibrateValues[2] << "for white"  << endl;
    cout << "Are you on starting spot? (yes?)" << endl;
    cin >> cont;
    while(cont != "yes"){
    	cout << endl;
    	cout << "Are you on starting spot? (yes?)" << endl;
    	cin >> cont;
    }
	cout << "finished";
    
    
    
 }

