#include "NodeTerminal.hpp"
NodeTerminal::NodeTerminal(){
    this->type = 1; 
    this->children = {nullptr,nullptr};
}
NodeTerminal::NodeTerminal(vector<char> options){
    this->children = {nullptr,nullptr};
    this->representation = 'x';
    this->type = 1;
    this->options = options;
}
float NodeTerminal::fitness(float input){
    return input;
}


void NodeTerminal::addChild(Node* child){
    return;
}

string NodeTerminal::print(){
    string aux (1,representation);
    return aux;
}