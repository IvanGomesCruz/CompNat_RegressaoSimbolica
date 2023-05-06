#include "NodeTerminal.hpp"
NodeTerminal::NodeTerminal(){
    this->type = 1; 
    this->children = {nullptr,nullptr};
}
NodeTerminal::NodeTerminal(string value,vector<string> options){
    this->children = {nullptr,nullptr};
    this->representation = value;
    if(value[0] != 'x'){
        this->terminalValue = stoi(value);
    }
    this->type = 1;
    this->options = options;
}
float NodeTerminal::f(float input){
    if(this->representation[0] = 'x'){
        return input;
    }
    return this->terminalValue;
}


void NodeTerminal::addChild(Node* child){
    return;
}

string NodeTerminal::print(){
    return this->representation;
}