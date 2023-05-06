#include "NodeTerminalConst.hpp"

float NodeTerminalConst::fitness(float input){
    return this->terminalValue;
}

NodeTerminalConst::NodeTerminalConst(char value,vector<char> options){
    this->options = options;
    this->representation = value;
    this->terminalValue = value - '0';
    
}