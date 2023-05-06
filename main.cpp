#include <iostream>
#include<vector>
#include <cstdlib>
#include "Node.hpp"
#include "NodeOperator.hpp"
#include "NodeTerminal.hpp"
#include "Tree.hpp"

#define DEPTHMAX 4
#define POPULATIONSIZE 10
const vector<char> operators = {'+', '-', '*','/'};
const vector<char> terminal = {'x','1','2','3','4','5'};

using namespace std;
//vector<Tree*> selection(int type,&vector<Tree*> population);
//Tree* selectionRoullete(&vector<Tree*> population);
//Tree* selectionTournament(&vector<Tree*> population, int k);
//Tree* selectionLexicase(&vector<Tree> population);

vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover);
vector<Tree*> initialPopulation(int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);


int main(int argc, char const *argv[]) {
    srand(1);
    vector<Tree*> population = initialPopulation(POPULATIONSIZE);
        population[1]->print();
        population[2]->print();
        population[1]->crossover(population[2]);
         population[1]->print();
        population[2]->print();
    
    return 0;
}

vector<Tree*> initialPopulation(int size){
    vector<Tree*> population;
    Tree* tree;
    while(population.size()<=size){
        if(population.size() <=size/2){
            
            tree = new Tree(generateFullRandomTree(0,population.size()%DEPTHMAX+1),DEPTHMAX);
            
        }
        else{
            tree = new Tree(generateGrowRandomTree(0,population.size()%DEPTHMAX+1),DEPTHMAX);
        }
            Tree::updateDepth(tree->root,0);
            population.push_back(tree);
    }

    return population;
}

Node* generateFullRandomTree(int depth,int depthmax) {
    int random;
    if(depth == depthmax){
        random = rand()%terminal.size();
       
        return new NodeTerminal(terminal[random],terminal);
        
    }
    random = rand() %operators.size();
    Node* node = new NodeOperator(operators[random],operators);
    node->addChild(generateFullRandomTree(depth+1,depthmax));
    node->addChild(generateFullRandomTree(depth+1,depthmax));
    return node;
}

Node* generateGrowRandomTree(int depth,int depthmax) {
    int random;
    random = rand()%2; //Determina se será Operador ou Terminal
    if(random ==1 || depth == DEPTHMAX){
        random = rand()%terminal.size();
      
        return new NodeTerminal(terminal[random],terminal);
        
    }
    random = rand() %operators.size();
    Node* node = new NodeOperator(operators[random],operators);
    node->addChild(generateGrowRandomTree(depth+1,depthmax));
    node->addChild(generateGrowRandomTree(depth+1,depthmax));
    return node;
}
vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover){
    int it=0;
    int random;
    while(it<selected.size()){
        random = (rand() % 100)/100;
        if(random<probMutation){
            selected[it]->mutation();
            it++;
        }
        else if(random>= probMutation && random<probMutation+probCrossover && it<selected.size()){ //A ultima condição é para o caso em que it esteja na ultima posição
           selected[it]->crossover(selected[it+1]);

            it+=2;
        }
        it+1;

    }
    return selected;
}