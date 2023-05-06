#ifndef NODETERMINALCONST_H
#define NODETERMINALCONST_H
#include "NodeTerminal.hpp"
#include "Node.hpp"

class NodeTerminalConst: public NodeTerminal
{
    public:
    float terminalValue;
    NodeTerminalConst(char value,vector<char> options);
    float fitness(float input) override;

};

#endif