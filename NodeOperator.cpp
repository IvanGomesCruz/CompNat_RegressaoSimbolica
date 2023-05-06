#include "NodeOperator.hpp"

NodeOperator::NodeOperator(char operatorType,vector<char> options){
    this->representation = operatorType;
    this->children = {nullptr,nullptr};
    this->options = options;
}

float NodeOperator::fitness(float input){
    float fitness;
    switch(this->representation){
        case '+': fitness = (*this->children[0]).fitness(input)+(*this->children[1]).fitness(input); break;
        case '-': fitness = (*this->children[0]).fitness(input)-(*this->children[1]).fitness(input); break;
        case '*': fitness = (*this->children[0]).fitness(input)*(*this->children[1]).fitness(input); break;
        case '/': 
            float aux1,aux2;
            aux1= (*this->children[0]).fitness(input);
            aux2= (*this->children[1]).fitness(input);
            if(aux2 ==0){
                fitness = 1;
            }
            else{
                fitness = aux1/aux2;
            }
            break;
        default: cout<<"undefined operator"<<endl;
    }
    return fitness;
}

void NodeOperator::addChild(Node* child){
    if(this->children[0] == nullptr){
        this->children[0] = child;
    }
    else if(this->children[1] == nullptr){
        this->children[1] = child;
    }
    child->depth = this->depth+1;
}


string NodeOperator::print(){
    string aux = "("+this->children[0]->print();
    string aux2(1,this->representation);
    string aux3 = this->children[1]->print();
    aux.append(aux2);
    aux.append(aux3);
    return aux + ")";
}

