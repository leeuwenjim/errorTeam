#include "Arnold.h"
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

Arnold arnold;

bool once = false;

vector<int> directions = {1, 0, 0, 2, 2, 0, 0, 1, 0, 1, 1, 0};

int main() {
	arnold.calibrate();
	int index = 0;
	while (true) {
		/*if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
			arnold.crossNavigator(directions[index]);
			++index;
			if (index > 11) {
				index = 0;
			}
		} else {
			arnold.lineFollowAlgoritm();
		}
		*//*
		arnold.crossNavigator(3);
		usleep(3000000);
		for(int i = 0; i < 1000; i++) {
			arnold.lineFollowAlgoritm();
		} */
		//arnold.lineFollowAlgoritm();
		
		arnold.turnDegrees(45);
		usleep(1000000);
		arnold.turnDegrees(90);
		usleep(1000000);
		arnold.turnDegrees(45);
		usleep(1000000);
		arnold.turnDegrees(-45);
		usleep(1000000);
		arnold.turnDegrees(-90);
		usleep(1000000);
		arnold.turnDegrees(-45);
		usleep(1000000);
	}
	
}
