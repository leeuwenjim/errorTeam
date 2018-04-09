#include <iostream>
#include <unistd.h>
#include "Arnold.h"

int main() {

	Arnold arnold;
	
		arnold.getUltrasonic();
		arnold.getLeftBW();
		arnold.getRightBW();
		arnold.turn_ultrasonic(1);
		sleep(5);
		arnold.turn_ultrasonic(2);

		
		
		
	
	
}
