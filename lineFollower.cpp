#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	int maxPower = 30;
	int currentLeftPower = 15;
	int currentRightPower = 15;
	
	int increments = 5;
	
	while(true) {
		
		if (arnold.leftSideOnLine()) {
			if (currentLeftPower > 0) {
				currentLeftPower -= increments;
			}
		} else {
			if (currentLeftPower < maxPower) {
				currentLeftPower += increments;
			}
		}
		
		if (arnold.rightSideOnLine()) {
			if (currentRightPower > 0) {
				currentRightPower -= increments;
			}
		} else {
			if (currentRightPower < maxPower) {
				currentRightPower += increments;
			}
		}
		
	}
	
}
