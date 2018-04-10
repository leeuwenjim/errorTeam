#include "Arnold.h"
#include <iosteam>

using namespace std;

Arnold arnold;

int main() {
	
	while (true) {
		if (arnold.leftSideOnLine() && arnold.rightSideOnLine()) {
			arnold.stop();
			arnold.crossNavigator(1);
			break;
		} else {
			arnold.move(20, 20);
		}
	}
	
}