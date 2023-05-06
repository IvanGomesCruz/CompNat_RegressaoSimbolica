#include "Node.hpp"

void Node::mutation(){
    cout<<"entra"<<endl;
    int random = rand()%this->options.size();

    this->representation = this->options[random];
    cout<<"sai"<<endl;
    
}