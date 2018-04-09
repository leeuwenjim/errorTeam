#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	int maxPower = 30;
	int minPower = -30;
	
	int currentLeftPower = 15;
	int currentRightPower = 15;
	
	
	int increments = 1;
	
	while(true) {
		bool leftSideOnLine = arnold.leftSideOnLine();
		bool rightSideOnLine = arnold.rightSideOnLine();
		
		if (leftSideOnLine && rightSideOnLine) {
			continue;
		}
		
		if (leftSideOnLine) {
			if (currentLeftPower > minPower) {
				currentLeftPower -= increments;
			}
		} else {
			if (currentLeftPower < maxPower) {
				currentLeftPower += increments;
			}
		}
		
		if (rightSideOnLine) {
			if (currentRightPower > minPower) {
				currentRightPower -= increments;
			}
		} else {
			if (currentRightPower < maxPower) {
				currentRightPower += increments;
			}
		}
		
		arnold.move(currentLeftPower, currentRightPower);
		
	}
	
}
