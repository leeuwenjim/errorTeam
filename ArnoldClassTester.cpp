#include <iostream>
#include <unistd.h>
#include "Arnold.h"

int main() {

	Arnold arnold;
	
	while (true) {
		arnold.getUltrasonic();
		arnold.getLeftBW();
		arnold.getRightBW();
		arnold.move(360, 360);
		sleep(1);
		arnold.move(0, 0);
	}
	
}
