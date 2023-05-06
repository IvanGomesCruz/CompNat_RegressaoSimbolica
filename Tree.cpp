#include "Tree.hpp"

Tree::Tree(Node* root){
    this->root = root;
    this->root->depth = 0;
    
}
float Tree::fitness(float input){
    return this->root->fitness(input);
}
void Tree::updateDepth(Node* node,int depth =0){
    if(node==nullptr) return;
    node->depth = depth;
    updateDepth(node->children[0],depth+1);
    updateDepth(node->children[1],depth+1);

    return;
}
void Tree::print(){
    cout<<this->root->print()<<endl;
}

int Tree::dephtTree(){
 return dephtTree(this->root);
}
int Tree::dephtTree(Node* node){
    if(node->children[0]==nullptr){
        return node->depth+1;
    }
    int depth1 = dephtTree(node->children[0]);
    int depth2 = dephtTree(node->children[1]);

    if(depth1 > depth2){
        return depth1;
    }
    return depth2;

}

bool Tree::mutation(Node* node){
    if(node = nullptr){
        return false;
    }
    int maxdepth = this->dephtTree();
    int random = rand()%maxdepth;
        cout<<node->depth<<endl;
    if(random<= node->depth){ // Probabilidade de mutação cresce conforme a profundidade
        node->mutation();

        return true;

    }
    random = rand()%2; ///Aletoriza o lado da arvore que vai ser percorrido primeiro, para evitar que um lado só seja mutado (quando o primeiro vizitado vai até a profundidade maxima)
    if(random==0){
        bool mutated = mutation(node->children[0]);
        if(mutated){
            return true;
        }
        mutated = mutation(node->children[1]);
        return mutated;
    }
    bool mutated = mutation(node->children[1]);
        if(mutated){
            return true;
        }
        mutated = mutation(node->children[0]);
        return mutated;

}
void Tree::mutation(){
    bool garbage = mutation(this->root);
}
