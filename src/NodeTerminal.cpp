#include "NodeTerminal.hpp"
NodeTerminal::NodeTerminal(){
    this->type = 1; 
    this->children = {nullptr,nullptr};
}
NodeTerminal::NodeTerminal(string value,vector<string> options){
    this->children = {nullptr,nullptr};
    this->representation = value;
    if(value[0] != 'x'){
        this->terminalValue = value[0]-'0';
    }
    this->type = 1;
    this->options = options;
}
float NodeTerminal::f(vector<float> input){
    if(this->representation[0] == 'x'){
        int indx =  stoi(this->representation.substr(1));
        return input[indx];
    }
    return this->terminalValue;
}


void NodeTerminal::addChild(Node* child){
    return;
}

string NodeTerminal::print(){
    return this->representation;
}
Node* NodeTerminal::copy(){
    Node* copy = new NodeTerminal(this->representation,this->options);
    return copy;
}