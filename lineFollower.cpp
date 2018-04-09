#include "Arnold.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	int maxPower = 40;
	int minPower = -15;
	
	int currentLeftPower = 15;
	int currentRightPower = 15;
	
	
	int increments = 1;
	
	while(true) {
		bool leftSideOnLine = arnold.leftSideOnLine();
		bool rightSideOnLine = arnold.rightSideOnLine();
		
		if (leftSideOnLine && rightSideOnLine) {
			usleep(55000);
		}
		
		usleep(1000);
		
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
