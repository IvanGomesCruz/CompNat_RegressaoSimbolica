#include "NodeTerminal.hpp"
NodeTerminal::NodeTerminal(){
    this->type = 1; 
    this->children = {nullptr,nullptr};
}
NodeTerminal::NodeTerminal(char value,vector<char> options){
    this->children = {nullptr,nullptr};
    this->representation = value;
    if(value != 'x'){
        this->terminalValue = value - '0';
    }
    this->type = 1;
    this->options = options;
}
float NodeTerminal::f(float input){
    if(this->representation = 'x'){
        return input;
    }
    return this->terminalValue;
}


void NodeTerminal::addChild(Node* child){
    return;
}

string NodeTerminal::print(){
    string aux (1,representation);
    return aux;
}