#include "NodeOperator.hpp"

NodeOperator::NodeOperator(string operatorType,vector<string> options){
    this->representation = operatorType;
    this->children = {nullptr,nullptr};
    this->options = options;
}

float NodeOperator::f(float input){
    float f;
    switch(this->representation[0]){
        case '+': f = (*this->children[0]).f(input)+(*this->children[1]).f(input); break;
        case '-': f = (*this->children[0]).f(input)-(*this->children[1]).f(input); break;
        case '*': f = (*this->children[0]).f(input)*(*this->children[1]).f(input); break;
        case '/': 
            float aux1,aux2;
            aux1= (*this->children[0]).f(input);
            aux2= (*this->children[1]).f(input);
            if(aux2 ==0){
                f = 1;
            }
            else{
                f = aux1/aux2;
            }
            break;
        default: cout<<"undefined operator"<<endl;
    }
    return f;
}

void NodeOperator::addChild(Node* child){
    if(this->children[0] == nullptr){
        this->children[0] = child;
    }
    else if(this->children[1] == nullptr){
        this->children[1] = child;
    }
    child->depth = this->depth+1;
    child->parent = this;
}


string NodeOperator::print(){
    string aux = "("+this->children[0]->print();
    string aux2 = this->representation;
    string aux3 = this->children[1]->print();
    aux.append(aux2);
    aux.append(aux3);
    return aux + ")";
}

