#include <vector>
#include <string>

using namespace std;

//NORTH = moveAdder[0]
//EAST = moveAdder[1]
//SOUTH = moveAdder[2]
//WEST = moveAdder[3]
int moveAdder [4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

class GridCell {
private:
	bool blocked;
	
public:
	GridCell() {
		blocked = false;
	}
	
	bool isBlocked() {
		return blocked;
	}
	
	void setBlocked(bool blocked) {
		this.blocked = blocked;
	}
	
};

class Arnold {
public:
	int x;
	int y;
	
	int direction = 0;
	
	void turn(string side) {
		if (side == "left") {
			direction--;
		} else if (side == "right") {
			direction++;
		}
		
		if (direction < 0) {
			direction += 4;
		}
		if (direction > 3) {
			direction -= 4;
		}
 	}
	
	void move() {
		x += moveAdder[direction][0];
		y += moveAdder[direction][1];
	}
	
}

vector<vector<GridCell> > grid;
Arnold arnold;

int main() {
	//INIT GRID SIZE
	int gridWidth = 5;
	int gridLength = 5;

	//INIT GRID
	for (unsigned int i = 0; i < gridLength; i++) {
		for (unsigned int j = 0; j < gridWidth; j++) {
			grid[i][j] = new GridCell();
		}
	}
	
	//MOVE ARNOLD TO NEXT SQUARE, UPDATE HIS POSITION, CHECK HIS ROUTE
	
	//MOVE ARNOLD WITH FUNCTION: 
	//arnold.move()
	
	//TURN ARNOLD TO THE LEFT WITH FUNCTION:
	//arnold.turn("left");
	//TURN ARNOLD TO TE RIGHT WITH FUNCTION:
	//arnold.turn("right");
}
