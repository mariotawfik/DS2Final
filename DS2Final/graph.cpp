//
//  graph.cpp
//  DS2Final
//
//  Created by Mary Tamry on 27.11.22.
//

#include "graph.hpp"
#include <iostream>

graph::graph()
{
    nodeType = 1;
    nodeID = 1;
    nodeName = (string) "router" + (string) "_" + to_string(nodeID);
    isInfected = false;
}

bool graph::changeNodeName(string customTag)
{
    if(customTag != "")
    {
        switch (nodeType)
        {
            case 1:
                nodeName = customTag  + (string) "_" + (string) "Router" + (string) "_" + to_string(nodeID);
                break;
            
            case 2:
                nodeName = customTag + (string) "_" + (string) "Switch" + (string) "_" + to_string(nodeID);
                break;
            
            case 3:
                nodeName = customTag + (string) "_" + (string) "Server" + (string) "_" + to_string(nodeID);
                break;
                
            case 4:
                nodeName = customTag + (string) "_" + (string) "PC" + (string) "_" + to_string(nodeID);
                break;
                
            default:
                break;
        }
    }
    
    return true;
}

graph* graph::insert(int type, bool customName)
{
    if(type == 1 || type == 2 || type == 3 || type == 4)
    {
        graph* x = new graph;
        
        
        
        connections.insert(connections.cend(), x);
        x->connections.insert(x->connections.cend(), this);
        
        switch (type)
        {
            case 1:
                x->nodeType = 1;
                x->nodeName = (string) "Router" + (string) "_" + to_string(nodeID);
                break;
            
            case 2:
                x->nodeType = 2;
                x->nodeName = (string) "Switch" + (string) "_" + to_string(nodeID);
                break;
            
            case 3:
                x->nodeType = 3;
                x->nodeName = (string) "Server" + (string) "_" + to_string(nodeID);
                break;
                
            case 4:
                x->nodeType = 4;
                x->nodeName = (string) "PC" + (string) "_" + to_string(nodeID);
                break;
                
            default:
                break;
        }
        
        if(customName == true)
        {
            string customTag;
            cout << "Please enter a custom tag " << endl;
            cin >> customTag;
            
            x->changeNodeName(customTag);
        }
        
        return x;
    }else{
        cout << "Please try again with a type value between 1 and 4" << endl;
        return nullptr;
    }
}

vector<graph*> *graph::completeExploration(vector<graph*> *prevPath)
{
    prevPath->push_back(this);
    if(connections.size() > 0)
    {
        for(int i = 0; i < connections.size(); i++)
        {
            bool explored = false;
            vector<graph*> copyOfPrevPath = *prevPath;
            for(int j = 0; j < prevPath->size() ; j++)
            {
                if(connections[i] == copyOfPrevPath.back())
                {
                    explored = true;
                }
                copyOfPrevPath.pop_back();
            }
            
            if(explored == false)
            {
                connections[i]->completeExploration(prevPath);
            }
        }
    }
    
    return prevPath;
}

bool graph::checkConnections()
{
    vector<graph*> *allNodes = new vector<graph*>;
    allNodes = completeExploration(allNodes);
    vector<graph*> copyOfNodes = *allNodes;
    int graphSize = allNodes->size();
    while(!allNodes->empty())
    {
        graph* pointA = allNodes->back();
        graph* pointZ;
        copyOfNodes = *allNodes;
        while(!copyOfNodes.empty())
        {
            pointZ = copyOfNodes.back();
            if(pointA != pointZ)
            {
                vector<graph*> currentPath;
                vector<vector<graph*>> paths;
                vector<graph*> &currentPathRef = currentPath;
                vector<vector<graph*>> &pathsRef = paths;
                if(checkAndCreateRedundancy(pointA->ptpExploration(currentPathRef, pathsRef, pointZ)) == 0)
                {
                    return false;
                }
                paths.clear();
            }
            copyOfNodes.pop_back();
        }
        allNodes->pop_back();
    }
    
    return true;
}

vector<vector<graph*>> graph::ptpExploration(vector<graph*> &currentPath, vector<vector<graph*>> &paths, graph* endNode)
{
    if(this == endNode){
        currentPath.push_back(this);
        paths.push_back(currentPath);
    }else{
        currentPath.push_back(this);
        for(int i = 0; i < connections.size(); i++)
        {
            bool explored = false;
            for(int k = 0; k < currentPath.size(); k++){
                if(connections[i] == currentPath[k])
                {
                    explored = true;
                }
            }
            if(explored == false)
            {
                connections[i]->ptpExploration(currentPath, paths, endNode);
            }
        }
    }
    currentPath.pop_back();
    
    return paths;
}

bool graph::addConnectionPath(graph* B)
{
    connections.push_back(B);
    B->connections.push_back(this);
    return true;
}

void graph::getNodes()
{
    vector<graph*> *path = new vector<graph*>;
    path = completeExploration(path);
    int graphSize = path->size();
    for(int i = 0; i < graphSize; i++)
    {
        cout << "Element " << i+1 << " = " << path->at(i)<< ", # of connections = " << path->at(i)->connections.size() << ", isInfected = " << path->at(i)->isInfected<< endl;
    }
}

bool graph::runSimulation()
{
    vector<graph*> *allNodes = new vector<graph*>;
    allNodes = completeExploration(allNodes);
    int max = allNodes->size()-1;
    int min = 0;
    srand(time(0));
    int randNum = rand()%(max-min + 1) + min;
    cout << "Before Infection: " << endl;
    getNodes();
    infectionRandomizer();
    cout << endl << "After Redundnacy Check: " << endl;
    checkConnections();
    getNodes();
    cout << endl << "After Infection & Deletion of Infected Node: " << endl;
    remove();
    checkConnections();
    getNodes();
    return true;
}

bool graph::checkAndCreateRedundancy(vector<vector<graph *>> paths)
{
    vector<pair<vector<graph*>, vector<graph*>>> disjointPaths;
    vector<pair<pair<vector<graph*>, vector<graph*>>, int>> otherPaths;
    if(paths.size() > 1)
    {
        for(int i = 0; i < paths.size(); i++)
        {
            for(int k = i+1; k < paths.size(); k++)
            {
                bool disjointVertices = true;
                int numOfCommonVertices = 0;
                for(int t = 1; t < paths[i].size()-1; t++)
                {
                    for(int l = 1; l < paths[k].size()-1; l++)
                    {
                        if(paths[i][t] == paths[k][l])
                        {
                            disjointVertices = false;
                            numOfCommonVertices++;
                        }
                    }
                }
                if(disjointVertices == true)
                {
                    disjointPaths.push_back(make_pair(paths[i], paths[k]));
                }else{
                    otherPaths.push_back(make_pair(make_pair(paths[i], paths[k]), numOfCommonVertices));
                }
            }
        }
        if(disjointPaths.empty())
        {
            pair<vector<graph*>, vector<graph*>> pathsWithLeastNumOfCommonVertices;
            int min = otherPaths[0].second;
            pathsWithLeastNumOfCommonVertices = otherPaths[0].first;
            for(int k = 1; k < otherPaths.size(); k++)
            {
                if(otherPaths[k].second < min)
                {
                    min = otherPaths[k].second;
                    pathsWithLeastNumOfCommonVertices = otherPaths[k].first;
                }
            }
            
            bool firstPathIsSmaller = true;
            vector<graph*> createdDisjointPath;
            createdDisjointPath.push_back(pathsWithLeastNumOfCommonVertices.first[0]);
            for(int i = 1; i < pathsWithLeastNumOfCommonVertices.first.size()-1; i++)
            {
                for(int k = 1; k < pathsWithLeastNumOfCommonVertices.second.size()-1; k++)
                {
                    if(pathsWithLeastNumOfCommonVertices.first.size() < pathsWithLeastNumOfCommonVertices.second.size())
                    {
                        if(pathsWithLeastNumOfCommonVertices.first[i] == pathsWithLeastNumOfCommonVertices.second[k])
                        {
                            graph* insertedNode = pathsWithLeastNumOfCommonVertices.first[i-1]->insert(2, false);
                            insertedNode->addConnectionPath(pathsWithLeastNumOfCommonVertices.first[i+1]);
                            pathsWithLeastNumOfCommonVertices.first[i] = insertedNode;
                            createdDisjointPath.push_back(insertedNode);
                        }else{
                            createdDisjointPath.push_back(pathsWithLeastNumOfCommonVertices.first[i]);
                        }
                    }else{
                        firstPathIsSmaller = false;
                        if(pathsWithLeastNumOfCommonVertices.first[i] == pathsWithLeastNumOfCommonVertices.second[k])
                        {
                            graph* insertedNode = pathsWithLeastNumOfCommonVertices.second[i-1]->insert(2, false);
                            insertedNode->addConnectionPath(pathsWithLeastNumOfCommonVertices.second[i+1]);
                            pathsWithLeastNumOfCommonVertices.second[i] = insertedNode;
                            createdDisjointPath.push_back(insertedNode);
                        }else{
                            createdDisjointPath.push_back(pathsWithLeastNumOfCommonVertices.second[k]);
                        }
                    }
                }
            }
            createdDisjointPath.push_back(pathsWithLeastNumOfCommonVertices.first.back());
            if(firstPathIsSmaller == true)
            {
                disjointPaths.push_back(make_pair(createdDisjointPath, pathsWithLeastNumOfCommonVertices.second));
            }else{
                disjointPaths.push_back(make_pair(createdDisjointPath, pathsWithLeastNumOfCommonVertices.first));
            }
        }
    }else{
        if(paths.size() == 1 && (paths[0].size() > 2))
        {
            paths[0].front()->addConnectionPath(paths[0].back());
            vector<graph*> newPath;
            newPath.push_back(paths[0].front());
            newPath.push_back(paths[0].back());
            disjointPaths.push_back(make_pair(paths[0], newPath));
            getNodes();
        }else{
            if(paths.size() == 1 && (paths[0].size() == 2))
            {
                return true;
            }
        }
    }
    
    if(disjointPaths.size() > 0){
        return true;
    }else{
        return false;
    }
}

graph* graph::remove()
{
    graph* infectedNode = nullptr;
    vector<graph*> *allNodes = new vector<graph*>;
    vector<graph*> &allNodesRef = *allNodes;
    allNodes = completeExploration(allNodes);
    for(int i = 0; i < allNodes->size(); i++)
    {
        if(allNodesRef[i]->isInfected == true)
        {
            infectedNode = allNodesRef[i];
            for(int j = 0; j < infectedNode->connections.size(); j++)
            {
                infectedNode->connections[j]->connections.erase(find(infectedNode->connections[j]->connections.begin(), infectedNode->connections[j]->connections.end(), infectedNode));
            }
            allNodesRef[i]->connections.clear();
        }
    }
    return infectedNode;
}

bool graph::infectionRandomizer()
{
    vector<graph*> *allNodes = new vector<graph*>;
    allNodes = completeExploration(allNodes);
    vector<graph*> &allNodesRef = *allNodes;
    int randomNodeIterator = numRandomizer();
    while(allNodesRef[randomNodeIterator] == this)
    {
        randomNodeIterator = numRandomizer();
    }
    allNodesRef[randomNodeIterator]->isInfected = true;
    return true;
}
          
int graph::numRandomizer()
{
    vector<graph*> *allNodes = new vector<graph*>;
    allNodes = completeExploration(allNodes);
    int max = allNodes->size()-1;
    int min = 0;
    srand(time(0));
    int randNum = rand()%(max-min + 1) + min;
    
    return randNum;
}
