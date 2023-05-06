#ifndef NODETERMINAL_H
#define NODETERMINAL_H
#include "Node.hpp"

class NodeTerminal: public Node
{
    public:
    NodeTerminal();
    float terminalValue;
    NodeTerminal(char value,vector<char> options);
    float f(float input) override;
    string print() override;
    void addChild(Node* child) override;

};



#endif