#ifndef NODETERMINAL_H
#define NODETERMINAL_H
#include "Node.hpp"

class NodeTerminal: public Node
{
    public:
    NodeTerminal();
    float terminalValue;
    NodeTerminal(string value,vector<string> options);
    float f(vector<float> input) override;
    string print() override;
    void addChild(Node* child) override;
    Node* copy() override;

};



#endif