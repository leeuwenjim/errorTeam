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

    leftW = arnold.getLeftBW();
    rightW = arnold.getRightBW();

    sleep(3);	
    arnold.move(20, 20);
    sleep(2);
    arnold.stop();

    leftW2 = arnold.getLeftBW();
    rightW2 = arnold.getRightBW();

    

    leftW3 = (leftW+leftW2)/2;
    rightW3 = (rightW+rightW2)/2;
    cout << "Left sensors white value is: " << leftW3 << endl;	
    cout << "Right sensors white value is: " << rightW3 << endl;


    return leftW3, rightW3;
}




int main(){

    
    
 }

