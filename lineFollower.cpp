#include "Arnold.h"
#include <iostream>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	
	while(true) {
		cout << arnold.leftSideOnLine()	<< endl;
	}
	
}
