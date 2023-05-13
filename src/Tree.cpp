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
    if(node==nullptr){
        this->depth = depth;
    return;
    } 
    node->depth = depth;
    updateDepth(node->children[0],depth+1);
    updateDepth(node->children[1],depth+1);

    return;
}
string Tree::print(){
    return this->root->print();
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

void Tree::crossover(Tree* partner){
    this->updateDepth(this->root,0);
    partner->updateDepth(partner->root,0);
    int partnerDepht = partner->dephtTree();
    int thisDepht = this->dephtTree();
    int height;
    int bigger = partnerDepht>thisDepht?partnerDepht:thisDepht;
        height = rand()%bigger; 
    if(height==0){
        height++;
    }

    ///determina uma altura para o crossover;
    vector<Node*> thisOptions = this->BFS(height);
    vector<Node*> thisOptionsOperators;
    vector<Node*> partnerOptions = partner->BFS(height);
    vector<Node*> partnerOptionsOperators;
     for(auto it:partnerOptions){
        if(it->type == 0){
           thisOptionsOperators.push_back(it);
        }
    }
    //gerar o indice dos Nodes a serem trocados
    for(auto it:partnerOptions){
        if(it->type == 0){
            partnerOptionsOperators.push_back(it);
        }
    }
    if(partnerOptionsOperators.size() == 0||thisOptionsOperators.size() ==0){
        return;
    }
    int i = rand()%((int)thisOptionsOperators.size());
    int j = rand()%((int)partnerOptionsOperators.size());
    //Troca os pontos

    vector<Node*> temp = thisOptionsOperators[i]->children;
    thisOptionsOperators[i]->children = partnerOptionsOperators[j]->children;
    partnerOptionsOperators[j]->children = temp;
}

vector<Node*> Tree::BFS(int depth){ // retorna os nós que estão na altura estabelecida
    vector<Node*>vector;
    queue<Node *> q;
    q.push(this->root);
    while (q.empty() == false) {
      Node* node = q.front();
      if(node->depth<=depth) {
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