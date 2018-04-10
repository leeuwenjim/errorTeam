#ifndef aStarSourceFile_h_
#define aStarSourceFile_h_
#include <algorithm>

// Coordinate struct to store XY coordinates.
struct Coordinate{
    unsigned int x;
    unsigned int y;
};

bool operator==(const Coordinate & lhs, const Coordinate & rhs);

// The class Node is used to store all the information for a node/cell in the grid.
class Node{
public:
    bool blocked;
    Coordinate coordinate;
    Coordinate parentCoordinate;
    unsigned int gCost; // Distance from source
    unsigned int hCost; // Distance from destination
    unsigned int fCost; // hCost + gCost
    
    Node();
    
    Node(bool _blocked, unsigned int _xCoordinate, unsigned int _yCoordinate);
    
    void updateFCost();
    
};

template<typename T>
bool isInVector(std::vector<T> vectorToUse, T variableToSearch){
    for(unsigned int i=0; i<vectorToUse.size(); i++){
        if(vectorToUse[i] == variableToSearch){
            return true;
        }
    }
    return false;
}

template<size_t xSize, size_t ySize>
std::vector<Coordinate> aStar(Node _currentNode, Node _grid[xSize][ySize], Node _destination){
    // This function is used to calculate a route from the _currentNode to the _destination Node on the _grid
    std::vector<Coordinate> openNodes = {_currentNode.coordinate}; // list with coordinates of Nodes that have a calculated fCost
    std::vector<Coordinate> closedNodes; // list with coordinates of Nodes that have been visited
    std::vector<Coordinate> neighbours; // list with coordinates of Nodes that are neighbouring the current Node
    std::vector<Coordinate> returnVector; // list with coordinates that form the route to the destination
    Node currentNode; // The Node that is currently being visited
    Node currentNeighbour; // The neighbouring Node whose fCost is currently being calculated
    unsigned int shortestFCost = 9999; // variable to store the smallest fCost
    unsigned int currentIndex = 0; // variable to store the index of the currentNode in the openNodes
    int counter = 0; // To detect when an infinite while(true) loop is running
    
    std::cout << "_currentCoordinate = " << _currentNode.coordinate.x << ' ' << _currentNode.coordinate.y << std::endl; // To monitor wich Node is used as origin
    // Code below calculates the hCost and updates the fCost of the origin Node, gCost is set to 0
    _grid[_currentNode.coordinate.x][_currentNode.coordinate.y].gCost = 0;
    _grid[_currentNode.coordinate.x][_currentNode.coordinate.y].hCost =
        (_currentNode.coordinate.x < _destination.coordinate.x)? (_destination.coordinate.x - _currentNode.coordinate.x) * 10 : (_currentNode.coordinate.x - _destination.coordinate.x)*10;
    _grid[_currentNode.coordinate.x][_currentNode.coordinate.y].updateFCost();
    
    // The while(true) loop below is the actual algorithm
    while(true){
        std::cout << "\n<<<<<<<<>>>>>>>>" << std::endl;
        
        shortestFCost = 9999; // reset the shortestFCost to a very high value.
        // Code below is used to detect and deal with a infinite loop
        if(counter > 25){
            std::cout << "infinite loop detected!" << std::endl;
            return returnVector;
        }
        std::cout << counter << std::endl;
        counter++;
        
        // Code below determines wich Node from the openNodes should be visited next, this is done through the lowest fCost
        for(unsigned int i=0; i<openNodes.size(); i++){
            if(_grid[openNodes[i].x][openNodes[i].y].fCost < shortestFCost){
                shortestFCost = _grid[openNodes[i].x][openNodes[i].y].fCost;
                std::cout << "lowest fcost = " << shortestFCost << std::endl;
                std::cout << "found at: " << openNodes[i].x << ' ' << openNodes[i].y << std::endl;
                currentIndex = i;
            }
        }
        
        currentNode = _grid[openNodes[currentIndex].x][openNodes[currentIndex].y];
        openNodes.erase(openNodes.begin() + currentIndex); // remove the current Node from the openNodes
        closedNodes.push_back(currentNode.coordinate); // add the current Node to the closedNodes
        
        std::cout << "Current Node = " << currentNode.coordinate.x << ' ' << currentNode.coordinate.y << std::endl;
        
        // If we have reached our destination, there is no reason to continue
        if(currentNode.coordinate == _destination.coordinate){
            std::cout << "Destination reached" << std::endl;
            break;
        }
        
        // The four code blocks below update the fCost and parent of the neighbours and update the openNodes
        if(currentNode.coordinate.x != 0){
            currentNeighbour = _grid[currentNode.coordinate.x - 1][currentNode.coordinate.y];
            if(currentNeighbour.blocked == false && isInVector(closedNodes, currentNeighbour.coordinate) == false){ // If the neighbour is blocked or the neighbour is already visited, there is no reason to calculate its fCost
                if(isInVector(openNodes, currentNeighbour.coordinate) == false || currentNode.gCost + 10 < currentNeighbour.gCost){
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].gCost = currentNode.gCost + 10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost =
                        (currentNeighbour.coordinate.x < _destination.coordinate.x)? (_destination.coordinate.x - currentNeighbour.coordinate.x) * 10 : (currentNeighbour.coordinate.x - _destination.coordinate.x)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost +=
                        (currentNeighbour.coordinate.y < _destination.coordinate.y)? (_destination.coordinate.y - currentNeighbour.coordinate.y) * 10 : (currentNeighbour.coordinate.y - _destination.coordinate.y)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].updateFCost();
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].parentCoordinate = currentNode.coordinate;
                    if(isInVector(openNodes, currentNeighbour.coordinate) == false){
                        openNodes.push_back(currentNeighbour.coordinate);
                        std::cout << "Added coordinate to openNodes: " << currentNeighbour.coordinate.x << ' ' << currentNeighbour.coordinate.y << std::endl;
                    }
                }
            }
        }
        if(currentNode.coordinate.x + 1 <= xSize - 1){
            currentNeighbour = _grid[currentNode.coordinate.x + 1][currentNode.coordinate.y];
            if(currentNeighbour.blocked == false && isInVector(closedNodes, currentNeighbour.coordinate) == false){
                if(isInVector(openNodes, currentNeighbour.coordinate) == false || currentNode.gCost + 10 < currentNeighbour.gCost){
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].gCost = currentNode.gCost + 10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost =
                        (currentNeighbour.coordinate.x < _destination.coordinate.x)? (_destination.coordinate.x - currentNeighbour.coordinate.x) * 10 : (currentNeighbour.coordinate.x - _destination.coordinate.x)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost +=
                        (currentNeighbour.coordinate.y < _destination.coordinate.y)? (_destination.coordinate.y - currentNeighbour.coordinate.y) * 10 : (currentNeighbour.coordinate.y - _destination.coordinate.y)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].updateFCost();
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].parentCoordinate = currentNode.coordinate;
                    if(isInVector(openNodes, currentNeighbour.coordinate) == false){
                        openNodes.push_back(currentNeighbour.coordinate);
                        std::cout << "Added coordinate to openNodes: " << currentNeighbour.coordinate.x << ' ' << currentNeighbour.coordinate.y << std::endl;
                    }
                }
            }
        }
        if(currentNode.coordinate.y != 0){
            currentNeighbour = _grid[currentNode.coordinate.x][currentNode.coordinate.y - 1];
            if(currentNeighbour.blocked == false && isInVector(closedNodes, currentNeighbour.coordinate) == false){
                if(isInVector(openNodes, currentNeighbour.coordinate) == false || currentNode.gCost + 10 < currentNeighbour.gCost){
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].gCost = currentNode.gCost + 10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost =
                        (currentNeighbour.coordinate.x < _destination.coordinate.x)? (_destination.coordinate.x - currentNeighbour.coordinate.x) * 10 : (currentNeighbour.coordinate.x - _destination.coordinate.x)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost +=
                        (currentNeighbour.coordinate.y < _destination.coordinate.y)? (_destination.coordinate.y - currentNeighbour.coordinate.y) * 10 : (currentNeighbour.coordinate.y - _destination.coordinate.y)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].updateFCost();
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].parentCoordinate = currentNode.coordinate;
                    if(isInVector(openNodes, currentNeighbour.coordinate) == false){
                        openNodes.push_back(currentNeighbour.coordinate);
                        std::cout << "Added coordinate to openNodes: " << currentNeighbour.coordinate.x << ' ' << currentNeighbour.coordinate.y << std::endl;
                    }
                }
            }
        }
        if(currentNode.coordinate.y + 1 <= ySize - 1){
            currentNeighbour = _grid[currentNode.coordinate.x][currentNode.coordinate.y + 1];
            if(currentNeighbour.blocked == false && isInVector(closedNodes, currentNeighbour.coordinate) == false){
                if(isInVector(openNodes, currentNeighbour.coordinate) == false || currentNode.gCost + 10 < currentNeighbour.gCost){
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].gCost = currentNode.gCost + 10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost =
                        (currentNeighbour.coordinate.x < _destination.coordinate.x)? (_destination.coordinate.x - currentNeighbour.coordinate.x) * 10 : (currentNeighbour.coordinate.x - _destination.coordinate.x)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].hCost +=
                        (currentNeighbour.coordinate.y < _destination.coordinate.y)? (_destination.coordinate.y - currentNeighbour.coordinate.y) * 10 : (currentNeighbour.coordinate.y - _destination.coordinate.y)*10;
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].updateFCost();
                    _grid[currentNeighbour.coordinate.x][currentNeighbour.coordinate.y].parentCoordinate = currentNode.coordinate;
                    if(isInVector(openNodes, currentNeighbour.coordinate) == false){
                        openNodes.push_back(currentNeighbour.coordinate);
                        std::cout << "Added coordinate to openNodes: " << currentNeighbour.coordinate.x << ' ' << currentNeighbour.coordinate.y << std::endl;
                    }
                }
            }
        }
    }
    counter = 0;
    
    // The while(true) loop below constructs the route/path from the origin Node to the destination Node
    while(true){
        counter++;
        if(counter > 25){
            break;
        }
        std::cout << "Generating path: " << std::endl;
        std::cout << currentNode.coordinate.x << ' ' << currentNode.coordinate.y << std::endl;
        if(currentNode.coordinate == _currentNode.coordinate){
            std::cout << "Route generated" << std::endl;
            break;
        }
        
        returnVector.push_back(currentNode.coordinate);
        currentNode = _grid[currentNode.parentCoordinate.x][currentNode.parentCoordinate.y];
    }
    
    std::reverse(returnVector.begin(),returnVector.end());
    return returnVector;
}


#endif
