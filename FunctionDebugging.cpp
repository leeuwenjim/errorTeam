#include <iostream>
#include <vector>
#include "aStarSourceFile.h"
//#include "Arnold.h"
#include <unistd.h>
using namespace std;

Coordinate coordinateArnold; // variable to store Arnold's current location
vector<Coordinate> route; // variable to store the route to the destination, this doesn't include the starting coordinate
unsigned int nodeCounter = 0; // variable used to count how far on the route Arnold is
unsigned int directionArnold; // variable to store Arnold's direction
vector<int> turnValuesArnold; // A vector that stores the input values used by the function crossNavigator

int minDistanceObstacles = 10;
uint16_t standardPowerValue = 10;

// variables to setup the grid
unsigned int gridSizeX = 5;
unsigned int gridSizeY = 5;
static Node grid[5][5]; // The array to store the grid

int tmp;

int getDirection(Coordinate currentCoordinate, Coordinate targetCoordinate){
    if(targetCoordinate.x != currentCoordinate.x){
        if(targetCoordinate.x - currentCoordinate.x > 0){
            return 1;
        }else{
            return 3;
        }
    }else if(targetCoordinate.y != currentCoordinate.y){
        cout << targetCoordinate.y << ' ' << currentCoordinate.y << endl;
        if(targetCoordinate.y - currentCoordinate.y > 0){
            cout << "returning 4:" << endl;
            cout << targetCoordinate.y << ' ' << currentCoordinate.y << endl;
            return 4;
        }else{
            return 2;
        }
    }else{
        cout << "Error at getDirection: currentNode IS the targetNode. currentNode = " << endl;
        cout << currentCoordinate.x << ' ' << currentCoordinate.y << endl;
        return 1;
    }
}

int directionsToTurnValue(const int & originDirection, const int & targetDirection){
    if(originDirection == targetDirection){
        return 1;
    }else if(originDirection == 1){
        if(targetDirection == 2){
            return 2;
        }else if(targetDirection == 4){
            return 0;
        }else{
            return 3;
        }
    }else if(originDirection == 4){
        if(targetDirection == 1){
            return 2;
        }else if(targetDirection == 3){
            return 0;
        }else{
            return 3;
        }
    }else{
        if(targetDirection == originDirection + 1){
            return 2;
        }else if(targetDirection == originDirection - 1){
            return 0;
        }else{
            return 3;
        }
    }
}

vector<int> generateTurnValues(const vector<Coordinate> & path, const int & startingDirection){
    vector<int> turnValues;
    int currentDirection = startingDirection;
    int targetDirection = getDirection(path[0], path[1]);
    turnValues.push_back(directionsToTurnValue(currentDirection, targetDirection));
    for(unsigned int i=2; i < path.size(); i++){
        currentDirection = targetDirection;
        targetDirection = getDirection(path[i-1], path[i]);
        turnValues.push_back(directionsToTurnValue(currentDirection, targetDirection));
    }
    
    return turnValues;
}


int main()
{ 
    // creating the grid
    for(unsigned int i=0; i<gridSizeX; i++){
        for(unsigned int j=0; j<gridSizeY; j++){
            grid[i][j].coordinate.x = i;
            grid[i][j].coordinate.y = j;
        }
    }
    
    Node Start = Node(false, 0, 4);
    Node Target = Node(false, 4, 0);
    
    route = aStar<5, 5>(Start, grid, Target);
    
    vector<int> turnVals = generateTurnValues(route, 1);
    
    cout << "<<<<>>>>" << endl;
    for(unsigned int i=0; i<turnVals.size(); i++){
        cout << turnVals[i] << ' ';
    }
    cout << endl;
    
    return 0;
}
