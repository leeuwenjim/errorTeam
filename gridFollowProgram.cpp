#include <iostream>
#include <vector>
#include "aStarSourceFile.h"
#include "Arnold.h"
#include <unistd.h>
using namespace std;

Arnold arnold;
Coordinate coordinateArnold; /// variable to store Arnold's current location
vector<Coordinate> route; /// variable to store the route to the destination, this doesn't include the starting coordinate
unsigned int nodeCounter = 0; /// variable used to count how far on the route Arnold is
unsigned int directionArnold; /// variable to store Arnold's direction
vector<int> turnValuesArnold; /// A vector that stores the input values used by the function crossNavigator

int minDistanceObstacles = 10;
uint16_t standardPowerValue = 30;

/// variables to setup the grid
unsigned int gridSizeX = 5;
unsigned int gridSizeY = 5;
static Node grid[5][5]; // The array to store the grid

int tmp;

/** function that returns wich direction Arnold should have if he is to move from currentCoordinate to targetCoordinate. The targetCoordinate schould be adjacent to the currentCoordinate.*/
int getDirection(Coordinate currentCoordinate, Coordinate targetCoordinate){
    if(targetCoordinate.x != currentCoordinate.x){
        if(targetCoordinate.x - currentCoordinate.x > 0){
            return 1;
        }else{
            return 3;
        }
    }else if(targetCoordinate.y != currentCoordinate.y){
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

/** function that returns wich inputValue should be given to the crossNavigator to turn Arnold from his originDirection to the targetDirection */
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

/** function that generates a vector containing all inputValues for the crossNavigator to correctly follow path */
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
    int userInputIntX; /// variable to store userinput
    int userInputIntY; /// variable to store more userinput
    int directionTargetStart = 1; /// variable to store wich direction Arnold is currently looking in
    float lasUltrasonicResult = 255.0; /// variable to store the result of the ultrasonic sensor
    
    /** Setup Arnold */
    arnold.calibrate();
    
    /// creating the grid
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
    
    Node originNode = Node(false, userInputIntX, userInputIntY); /// Set the originNode
    coordinateArnold = originNode.coordinate; /// Set Arnolds current coordinate
    
    cout << "Give the X destination coordinate: " << endl;
    cin >> userInputIntX;
    cout << "Give the Y destination coordinate: " << endl;
    cin >> userInputIntY;
    
    Node destinationNode = Node(false, userInputIntX, userInputIntY); /// Set the destinationNode
    
    cout<< "Give Arnold's direction:" << endl
        << "1 = X+" << endl
        << "2 = Y-" << endl
        << "3 = X-" << endl
        << "4 = Y+" << endl;
    cin >> directionArnold; /// Set Arnold's current direction
    
    route = aStar<5, 5>(originNode, grid, destinationNode); /// Generate the route to the destination
    
    directionTargetStart = getDirection(coordinateArnold, route[0]); /// Wich direction should Arnold be looking in at the start of the route
    /** code below makes sure Arnold is looking in the correct direction */
    if(directionArnold != directionTargetStart){
        cout << "Turning to the correct direction" <<endl;
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
    
    turnValuesArnold = generateTurnValues(route, directionArnold); /// Generate the turnValues (inputValues) to be used by the crossNavigator
    cout << directionArnold << endl;
    cout << "TurnValues:" << endl;
    for(unsigned int j=0; j<turnValuesArnold.size(); j++){
        cout << turnValuesArnold[j] << ' ';
    }
    cout << endl;
    
    while(not (coordinateArnold == destinationNode.coordinate)){
        lasUltrasonicResult = arnold.getUltrasonic(); /// Save the result of the ultrasonic sensor
        /** The if-statements below determine what Arnold should do this iteration:
         * if both the left and right sensor return true, then both "see" black. This means that a new Node has been reached.
         * if the ultrasonicSensor detects an object less than minDistanceObstacles cm away, then this will be interpreted as an obstacle.
         * * Arnold will then move backwards until a Node has been reached. This Node will be interpreted as the last Node.
         * * Arnold will then update the grid with this new obstacle.
         * * Arnold will then calculate a new route to the destination.
         * * Arnold will then turn to the correct direction, and continue his journey.
         * if nothing special happens, Arnold should keep following the line.
        */
        if(arnold.leftSideOnLine() && arnold.rightSideOnLine()){
            cout << "New Node reached!" << endl;
            coordinateArnold = route[nodeCounter];
            if(coordinateArnold == destinationNode.coordinate){
                break;
            }
            arnold.crossNavigator(turnValuesArnold[nodeCounter]);
            directionArnold = getDirection(route[nodeCounter], route[nodeCounter + 1]);
            nodeCounter++;
        }else if(lasUltrasonicResult <= minDistanceObstacles && arnold.getUltrasonic() > 2){
            cout << "Obstacle detected!!" << endl;
            cout << lasUltrasonicResult << "cm" << endl;
            arnold.move(-standardPowerValue, -standardPowerValue); /// Move backwards, away from the obstacle
            grid[route[nodeCounter].x][route[nodeCounter].y].blocked = true; /// update the grid
            while(not(arnold.leftSideOnLine() && arnold.rightSideOnLine())){
                usleep(100000);
            }
            arnold.stop(); /// when a Node has been reached, stop Arnold and move him forward until he's centered on the Node
            arnold.move(40, 40);
            usleep(800000);
            arnold.stop();
            route = aStar<5, 5>(grid[coordinateArnold.x][coordinateArnold.y], grid, destinationNode); /// calculate the new route
            nodeCounter = 0; /// reset the NodeCounter
            /// code below makes sure Arnold is looking in the correct direction
            directionTargetStart = getDirection(coordinateArnold, route[0]);
            if(directionArnold != directionTargetStart){
                cout << "Turning to the right direction" <<endl;
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
                
                turnValuesArnold = generateTurnValues(route, directionArnold); /// generate the new turnValues for the newly calculated route
                cout << "Current coordinate: " << coordinateArnold.x << ' ' << coordinateArnold.y << endl;
            }
            continue;
        }else{
            arnold.lineFollowAlgoritm();
        }
    }

    arnold.reset();
    
	return 0;
}
