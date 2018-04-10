#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

bool once = false;

vector<int> directions = {1, 0, 0, 2, 2, 0, 0, 1, 0, 1, 1, 0};
int index = 0;

int main() {
	arnold.calibrate();
	while (true) {
		if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
			arnold.crossNavigator(directions[i]);
			++i;
			if (i > 11) {
				i = 0;
			}
		} else {
			arnold.lineFollowAlgoritm();
		}
	}
	
}