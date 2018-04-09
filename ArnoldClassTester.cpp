#include <iostream>
#include <unistd.h>
#include "Arnold.h"

int main() {

	Arnold arnold;
	
	while (true) {
		arnold.getUltrasonic();
		arnold.getLeftBW();
		arnold.getRightBW();
		arnold.move(50, 50);
		sleep(1);
		arnold.stop();
	}
	
}
