#ifndef NODEOPERATOR_H
#define NODEOPERATOR_H

#include "Node.hpp"

class NodeOperator: public Node
{
    public:
    NodeOperator(string operatorType,vector<string> options);
    float f(float input) override;
    void addChild(Node* child);
    string print() override;
};


#endif