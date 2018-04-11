#include <iostream>
#include <vector>
#include "aStarSourceFile.h"
#include "Arnold.h"
#include <unistd.h>
using namespace std;

Arnold arnold;
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
        if(targetCoordinate.y - currentCoordinate.y > 0){
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
    int userInputIntX;
    int userInputIntY;
    int directionTargetStart = 1;
    
    // Setup Arnold
    arnold.calibrate();
    
    // creating the grid
    for(unsigned int i=0; i<gridSizeX; i++){
        for(unsigned int j=0; j<gridSizeY; j++){
            grid[i][j].coordinate.x = i;
            grid[i][j].coordinate.y = j;
        }
    }
    
    cout << "Give the X starting coordinate: " << endl;
    cin >> userInputIntX;
    cout << "Give the Y starting coordinate: " << endl;
    cin >> userInputIntY;
    
    Node originNode = Node(false, userInputIntX, userInputIntY);
    coordinateArnold = originNode.coordinate;
    
    cout << "Give the X destination coordinate: " << endl;
    cin >> userInputIntX;
    cout << "Give the Y destination coordinate: " << endl;
    cin >> userInputIntY;
    
    Node destinationNode = Node(false, userInputIntX, userInputIntY);
    
    cout<< "Give Arnold's direction:" << endl
        << "1 = X+" << endl
        << "2 = Y-" << endl
        << "3 = X-" << endl
        << "4 = Y+" << endl;
    cin >> directionArnold;
    
    route = aStar<5, 5>(originNode, grid, destinationNode);
    turnValuesArnold = generateTurnValues(route, directionArnold);
    
    directionTargetStart = getDirection(coordinateArnold, route[0]);
    if(directionArnold != directionTargetStart){
        tmp = directionsToTurnValue(directionArnold, directionTargetStart);
        if(tmp == 0){
            arnold.turn(1);
            if(directionArnold == 1){
                directionArnold = 4;
            }else{
                directionArnold--;
            }
        }else if(tmp == 2){
            arnold.turn(2);
            if(directionArnold == 4){
                directionArnold = 1;
            }else{
                directionArnold++;
            }
        }else if(tmp == 3){
            arnold.turn(1);
            arnold.turn(1);
            if(directionArnold < 3){
                directionArnold += 2;
            }else{
                directionArnold -= 2;
            }
        }else{
            cout << "Error at turning to the first Node of path" << endl;
        }
    }
    
    while(not (coordinateArnold == destinationNode.coordinate)){
        if(arnold.leftSideOnLine() && arnold.rightSideOnLine()){
            coordinateArnold = route[nodeCounter];
            if(coordinateArnold == destinationNode.coordinate){
                break;
            }
            arnold.crossNavigator(turnValuesArnold[nodeCounter]);
            directionArnold = getDirection(route[nodeCounter], route[nodeCounter + 1]);
            nodeCounter++;
        }else if(arnold.getUltrasonic() <= minDistanceObstacles){
            arnold.move(-standardPowerValue, -standardPowerValue);
            grid[route[nodeCounter].x][route[nodeCounter].y].blocked = true;
            while(not(arnold.leftSideOnLine() && arnold.rightSideOnLine())){
                usleep(100000);
            }
            arnold.stop();
            route = aStar<5, 5>(grid[coordinateArnold.x][coordinateArnold.y], grid, destinationNode);
            nodeCounter = 0;
            directionTargetStart = getDirection(coordinateArnold, route[0]);
            if(directionArnold != directionTargetStart){
                tmp = directionsToTurnValue(directionArnold, directionTargetStart);
                if(tmp == 0){
                    arnold.turn(1);
                    if(directionArnold == 1){
                        directionArnold = 4;
                    }else{
                        directionArnold--;
                    }
                }else if(tmp == 2){
                    arnold.turn(2);
                    if(directionArnold == 4){
                        directionArnold = 1;
                    }else{
                        directionArnold++;
                    }
                }else if(tmp == 3){
                    arnold.turn(1);
                    arnold.turn(1);
                    if(directionArnold < 3){
                        directionArnold += 2;
                    }else{
                        directionArnold -= 2;
                    }
                }else{
                    cout << "Error at turning to the first Node of path" << endl;
                }
            }
            continue;
        }else{
            arnold.lineFollowAlgoritm();
        }
    }
    
	return 0;
}
