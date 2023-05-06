#include "Node.hpp"

void Node::mutation(){
    int random = rand()%this->options.size();
    
    while(this->options[random] == this->representation){ //evita que troque para o mesmo
        random = rand()%this->options.size();
    }
    this->representation = this->options[random];
    
}