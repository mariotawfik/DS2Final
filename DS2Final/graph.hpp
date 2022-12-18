//
//  graph.hpp
//  DS2Final
//
//  Created by Mary Tamry on 27.11.22.
//

#ifndef graph_hpp
#define graph_hpp

using namespace std;
#include <stdio.h>
#include <array>
#include <vector>
#include <utility>
#include <string>
#include <queue>
#include <stack>

class graph{
private:
    string nodeName; //Assigned using the nodeType and the nodeID
    int nodeType; //The node's type; 1 = router, 2 = switch, 3 = server, 4 = PC
    int nodeID; //Automatically assigned ID based on the other devices IDs
    bool isInfected; //A boolean indicator of weather this node is infected or not (used for a similation and test scenario of an always available network.
    vector<graph*> connections; //A vector with pairs of a pointer to the connected node
    bool changeNodeName(string customTag); //Changes the nodeName using the nodeType and the nodeID, takes in a customeTag as a paramater;
    bool infectionRandomizer(); //A random algorithm to infect only one node (checks if there is another node infected first then infects a random node). Returns true if infection is complete.
    bool checkConnections(); //Checks connections in the graph are redundant
    bool checkAndCreateRedundancy(vector<vector<graph*>> paths); //Checks wether the function has disjoint paths which disjoint vertices, if it doesnt then it checks for the paths with the least number of common nodes and creates nodes and inserts it in the path accordingly in order to create redundancy.
    vector<graph*>* completeExploration(vector<graph*> *previousPath); //Gets all the nodes in the graph and adds them in a queue.
    vector<vector<graph*>> ptpExploration(vector<graph*> &currentPath, vector<vector<graph*>> &paths, graph* endNode); //Creates a vector of all the paths from the current node to another specified node
    int numRandomizer(); //Returns a random number that is between 0 and the number of nodes. There is an exception for the node that it is currently being called from. Thus, it cannnot be that.
public:
    graph(); //Constructor
    graph* insert(int type, bool assignCustomName); // Insertion with the node type and a boolean indicator to weather the user wants to add a custom tag to the nodeName. Returns true if insertion is complete.
    graph* remove(); //returns the removed node.
    void getNodes(); //Prints all the nodes in the network
    bool runSimulation(); //Runs an infectiom similuation, makes sure there is redundant links and cuts the infected node, uninfects the node then inserts it again in its position.
    bool addConnectionPath(graph* B); //adds a connection path between the current node and node B. returns true if successdully added.
};



#endif /* graph_hpp */
