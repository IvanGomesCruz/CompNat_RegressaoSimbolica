#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include "InputConfig.hpp"

using namespace std;

//----------------------------------------------------------------
// Type definitions: 
// 0- terminal
// 1- Operator


class Node
{
    public:
    int depth;
    int type;
    string representation;
    Node* parent;
    virtual float f(vector<float> input) = 0;
    vector<Node*> children;
    virtual string print() =0;
    virtual void addChild(Node* child) =0;
    virtual Node* copy() =0;
    vector<string> options;
    void mutation();
    virtual ~Node();
};


#endif