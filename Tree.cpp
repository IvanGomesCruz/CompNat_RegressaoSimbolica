#include "Tree.hpp"

Tree::Tree(Node* root,int maxDepht){
    this->MaxDepth = maxDepht;

    this->root = root;
    this->root->depth = 0;
    
}
float Tree::f(vector<float> input){
    return this->root->f(input);
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
    if(node == nullptr){
        return false;
    }
    int random = rand()%7;
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
    mutation(this->root);
}

void Tree::crossover(Tree* partiner){
    int partnerDepht = partiner->dephtTree();
    ///determina uma altura para o crossover;
    int height;
    height = rand()%7; 


    vector<Node*> thisOptions = this->BFS(height);
    vector<Node*> partnerOptions = partiner->BFS(height);

    //gerar o indice dos Nodes a serem trocados
    int i = rand()%thisOptions.size();
    int j = rand()%partnerOptions.size();

    //Troca os pontos
    Node* aux = thisOptions[i]->parent;
    thisOptions[i]->parent = partnerOptions[j]->parent;
    if(partnerOptions[j]->parent->children[0]==partnerOptions[j]){
        partnerOptions[j]->parent->children[0] = thisOptions[i];
    }
    else{
         partnerOptions[j]->parent->children[1] = thisOptions[i];
    }

    partnerOptions[j]->parent = aux->parent;
     if(aux->children[0]==thisOptions[i]){
        aux->children[0] = partnerOptions[j];
    }
    else{
         aux->children[1] = partnerOptions[j];
    } 
   

}

vector<Node*> Tree::BFS(int depth){ // retorna os nós que estão na altura estabelecida
    vector<Node*>vector;
    queue<Node *> q;
    q.push(this->root);
    while (q.empty() == false) {
      Node* node = q.front();
      if(node->depth==depth) {
        vector.push_back(node);
      }
      q.pop();
      if(node->depth<=depth){
        if (node->children[0] != NULL)
            q.push(node->children[0]);
        if (node->children[1] != NULL)
            q.push(node->children[1]);
      }
   }
   return vector;
}

Tree* Tree::copy(){
    Tree* copy = new Tree(this->root->copy(),this->MaxDepth);
    return copy;
}