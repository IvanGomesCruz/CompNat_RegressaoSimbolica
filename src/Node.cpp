#include "Node.hpp"

void Node::mutation(){
    
    int random = rand()%this->options.size();
    
    while(this->options[random] == this->representation){ //evita que troque para o mesmo
        random = rand()%this->options.size();
    }
    this->representation = this->options[random];
    
}
Node::~Node(){
    if(children[0]!= nullptr){
        delete this->children[0];
        delete this->children[1];
    }
}