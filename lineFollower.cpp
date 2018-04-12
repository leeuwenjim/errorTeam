#include "Arnold.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;

int main() {
	///Calibrate code to ensure light sensors are set up right.
	arnold.calibrate();
	
	/** While loop will continually check ultrasonic, if it sees an obstacle it'll start obstacle avoid program. Any other moment it
	* will run the line follow algoritm. */
	while(true) {
		if (arnold.getUltrasonic() < 10) { /// Ensures Arnold won't drive against obstacles
			arnold.goAroundObstacle();
            cout << "eind object ontwijken" << endl;
				
		} 
		else{ 	/// If there isn't an object in sight Arnold will follow the line
            cout << "begin lijnvolg" << endl;
			arnold.lineFollowAlgoritm();
		}
		
	}
	
}
