#include "Arnold.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	
	while(true) {
		if (arnold.getUltrasonic() < 10) {
			arnold.goAroundObstacle();
				
		} 
		
		arnold.lineFollowAlgoritm();
		
	}
	
}
