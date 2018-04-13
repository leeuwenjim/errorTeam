#include <iostream>
#include <vector>
#include <algorithm>
#include "aStarSourceFile.h"
using namespace std;

bool operator==(const Coordinate & lhs, const Coordinate & rhs){
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/**
* Default constructor for the Node class
*/
Node::Node(){
    this -> blocked = false;
    this -> gCost = 0;
    this -> hCost = 0;
    this -> fCost = 0;
}

/**
* _blocked: sets the Node blocked if true, unblocked if false, _xCoordinate: sets the x coordinate for this Node, _yCoordinate: sets the y coordinate for this Node
*
* Constructor for the Node class
*/
Node::Node(bool _blocked, unsigned int _xCoordinate, unsigned int _yCoordinate){
    this -> blocked = _blocked;
    this -> coordinate.x = _xCoordinate;
    this -> coordinate.y = _yCoordinate;
    this -> gCost = 0;
    this -> hCost = 0;
    this -> fCost = 0;
}

/**
* calculates and set the fCost of the Node
*/
void Node::updateFCost(){
    this -> fCost = this -> gCost + this -> hCost;
}
