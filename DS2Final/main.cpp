//
//  main.cpp
//  DS2Final
//
//  Created by Mary Tamry on 27.11.22.
//

#include <iostream>
#include "graph.hpp"

int main()
{
    graph* A = new graph;
    graph* B = A->insert(2, false);
    graph* C = B->insert(2, false);
    graph* D = C->insert(2, false);
    graph* F = B->insert(2, false);
    graph* E = F->insert(2, false);
    graph* G = C->insert(2, false);
    graph* H = B->insert(2, false);
    F->addConnectionPath(C);
    F->addConnectionPath(D);
    A->runSimulation();
}
