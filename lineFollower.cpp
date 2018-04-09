#include "Arnold.h"

Arnold arnold;

int main() {

	arnold.calibrate();
	
	while(true) {
		cout << arnold.leftSideOnLine()	<< endl;
	}
	
}
