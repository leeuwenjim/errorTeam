#include "Arnold.cpp"
#include <iostream>
#include <unistd.h>

using namespace std;

Arnold arnold;

int main() {
	while(true) {
	cout << arnold.getUltrasonic() << endl;
		usleep(1000000);
	}
}
