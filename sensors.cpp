#include "Arnold.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;
/** ultrasonic test print*/
int main() {
	while(true) {
	cout << arnold.getUltrasonic() << endl;
		usleep(1000000);
	}
}
