#include <iostream>
#include <unistd.h>
#include <Arnold.h>

int main() {

	Arnold arnold;
	
	while (true) {
		arnold.getUltrasonic();
		arnold.getLeftBW();
		arnold.getRightBW();
		sleep(1);
	}
	
}
