#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include "ConfigEntrada.hpp"

using namespace std;

//----------------------------------------------------------------
// Type definitions: 
// 1- terminal
// 2- Operator


class Node
{
    public:
    int depth;
    int type;
    string representation;
    Node* parent;
    virtual float f(float input) = 0;
    vector<Node*> children;
    virtual string print() =0;
    virtual void addChild(Node* child) =0;
    vector<string> options;
    void mutation();
};


#endif