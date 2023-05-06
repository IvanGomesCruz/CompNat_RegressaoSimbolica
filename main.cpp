#include <iostream>
#include<vector>
#include <cstdlib>
#include "Node.hpp"
#include "NodeOperator.hpp"
#include "NodeTerminal.hpp"
#include "NodeTerminalConst.hpp"
#include "Tree.hpp"

#define DEPTHMAX 4
#define POPULATIONSIZE 3
const vector<char> operators = {'+', '-', '*','/'};
const vector<char> terminal = {'x','1','2','3','4','5'};

using namespace std;
//vector<Tree*> selection(int type,&vector<Tree*> population);
//Tree* selectionRoullete(&vector<Tree*> population);
//Tree* selectionTournament(&vector<Tree*> population, int k);
//Tree* selectionLexicase(&vector<Tree> population);
//Tree mutation(Tree subject);
//vector<Tree*> crossover(&Tree parent1, &Tree parent2);
vector<Tree*> initialPopulation(int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);


int main(int argc, char const *argv[]) {
    srand(1);
    vector<Tree*> population = initialPopulation(POPULATIONSIZE);
    population[3]->print();
    cout<<"entra"<<endl;
    population[3]->mutation();
    cout<<"sai"<<endl;
    population[3]->print();
    return 0;
}

vector<Tree*> initialPopulation(int size){
    vector<Tree*> population;
    Tree* tree;
    while(population.size()<=size){
        if(population.size() <=size/2){
            
            tree = new Tree(generateFullRandomTree(0,population.size()%DEPTHMAX));
            
        }
        else{
            tree = new Tree(generateGrowRandomTree(0,population.size()%DEPTHMAX));
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
        if(terminal[random]=='x'){
            return new NodeTerminal();
        }
        else{
            return new NodeTerminalConst(terminal[random],terminal);
        }
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
        if(terminal[random]=='x'){
            return new NodeTerminal();
        }
        else{
            return new NodeTerminalConst(terminal[random],terminal);
        }
    }
    random = rand() %operators.size();
    Node* node = new NodeOperator(operators[random],operators);
    node->addChild(generateGrowRandomTree(depth+1,depthmax));
    node->addChild(generateGrowRandomTree(depth+1,depthmax));
    return node;
}