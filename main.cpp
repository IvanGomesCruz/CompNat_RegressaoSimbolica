#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
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
Tree* selectionRoulette(vector<Tree*> population,vector<float>& input,vector<float>&realValues);
Tree* selectionTournament(vector<Tree*> population,vector<float>& input,vector<float>&realValues,int k);
//Tree* selectionLexicase(&vector<Tree> population);

vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover);
vector<Tree*> initialPopulation(int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);
float eval(Tree* subject,float input);


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

float fitness(Tree* subject,vector<float>& input,vector<float>&realValues){
    int N = input.size();
    float sumEval =0;
   for(int i=0;i<N;i++){
        sumEval += pow(eval(subject,input[i])-realValues[i],2);
   }
   float RSME = (1/N)*sumEval;
    return pow(RSME,1/2);
}

float eval(Tree* subject,float input){
    return subject->f(input);
}


Tree* selectionRoulette(vector<Tree*> population,vector<float>& input,vector<float>&realValues){
    vector<pair<float,Tree*>> roulette;
    float fitnessTotal =0;
    float fitnessInd;
    for(int i=0;i<population.size();i++){
        fitnessInd = fitness(population[i],input,realValues);
        roulette.push_back(make_pair(fitnessInd,population[i]));
        fitnessTotal += fitnessInd;
    }
    for(int i = 0;i<roulette.size();i++){
        roulette[i].first = roulette[i].first/fitnessTotal;
        if(i!=0){
            roulette[i].first += roulette[i-1].first;
        }
        std::sort(roulette.begin(),roulette.end());
    }
    float random= (rand()%(int)fitnessTotal)/fitnessTotal;
    int j =0;
    while(random>roulette[j].first){
        j++;
    }
    return roulette[j].second;

}

Tree* selectionTournament(vector<Tree*> population,vector<float>& input,vector<float>&realValues,int k){
    Tree* bestTree = nullptr;
    int random=0;
    for(int i=0;i<k;i++){
        random = rand()%population.size();
        if (bestTree == nullptr){
            bestTree = population[random];
        }
        if(fitness(bestTree,input,realValues)<fitness(population[random],input,realValues)){
            bestTree = population[random];
        }
    }
    return bestTree;
}