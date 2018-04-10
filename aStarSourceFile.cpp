#include <iostream>
#include <vector>
#include <algorithm>
#include "aStarSourceFile.h"
using namespace std;

bool operator==(const Coordinate & lhs, const Coordinate & rhs){
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Node::Node(){
    this -> blocked = false;
    this -> gCost = 0;
    this -> hCost = 0;
    this -> fCost = 0;
}

Node::Node(bool _blocked, unsigned int _xCoordinate, unsigned int _yCoordinate){
    this -> blocked = _blocked;
    this -> coordinate.x = _xCoordinate;
    this -> coordinate.y = _yCoordinate;
    this -> gCost = 0;
    this -> hCost = 0;
    this -> fCost = 0;
}

void Node::updateFCost(){
    this -> fCost = this -> gCost + this -> hCost;
}
