#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

int main() {
	arnold.calibrate();
	while (true) {
		if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
			arnold.stop();
			arnold.crossNavigator(0);
			break;
		} else {
			arnold.move(20, 20);
		}
	}
	
}