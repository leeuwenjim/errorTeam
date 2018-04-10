#include "BrickPi3.h"
#include <unistd.h>

Arnold arnold;

int main() {

	
}

void goAroundObstacle() {
    arnold.turn(2);
    arnold.turn_ultrasonic(1);
    arnold.move(40,40);
    if(arnold.getUltrasonic() > 15) {
        sleep(1);
        arnold.stop();
    }
    arnold.turn(1);
    arnold.move(40,40);
    if(arnold.getUltrasonic() > 10) {
        sleep(1);
        arnold.stop();
    }
    arnold.turn(1);
    arnold.move(40,40);
    while(true) {
        if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
            arnold.stop();
            arnold.crossNavigator(2);
            break;
        } else {
            arnold.move(20, 20);
        }
    }
}
	
