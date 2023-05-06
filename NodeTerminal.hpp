#ifndef NODETERMINAL_H
#define NODETERMINAL_H
#include "Node.hpp"

class NodeTerminal: public Node
{
    public:
    NodeTerminal();
    NodeTerminal(vector<char> options);
    float fitness(float input) override;
    string print() override;
    void addChild(Node* child) override;

};



#endif