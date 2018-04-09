#include "Arnold.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;

int main() {

	arnold.calibrate();
	
	while(true) {
		arnold.lineFollowAlgoritm();
	}
	
}
