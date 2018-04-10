#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

bool once = false;

int main() {
	arnold.calibrate();
	while (true) {
		if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
			arnold.stop();
			if (once) {
				arnold.crossNavigator(0);
				break;
			} else {
				arnold.crossNavigator(1);
				once = true;
			}
		} else {
			arnold.move(20, 20);
		}
	}
	
}