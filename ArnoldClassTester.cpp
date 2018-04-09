#include <iostream>
#include <unistd.h>
#include "Arnold.h"

int main() {

	Arnold arnold;
	
		arnold.getUltrasonic();
		arnold.getLeftBW();
		arnold.getRightBW();
		arnold.turn_ultrasonic(2);
		
		arnold.move(30, 30);
		sleep(1);
		arnold.stop();
		arnold.move(60, 0);
		sleep(2);
		arnold.stop();
		
		
		
	
	
}
