#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include "Node.hpp"
#include "NodeOperator.hpp"
#include "NodeTerminal.hpp"
#include "Tree.hpp"
#include "ConfigEntrada.hpp"

#define DEPTHMAX 7
#define POPULATIONSIZE 10
const vector<string> operators = {"+","-","*","/"};
//const vector<string> terminal = {"x","1","2","3"};

using namespace std;
vector<Tree*> selection(int type,vector<Tree*> population,vector<vector<float>>& input);
vector<Tree*> selectionRoulette(vector<Tree*> population,vector<vector<float>>& input);
Tree* selectionTournament(vector<Tree*> population,vector<vector<float>>& input,int k);
//Tree* selectionLexicase(&vector<Tree> population);
float fitness(Tree* subject,vector<vector<float>>& dataset);
vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover);
vector<Tree*> initialPopulation(int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);
float eval(Tree* subject,vector<float> input);
Tree* evoluition(int generation,vector<Tree*>population,vector<vector<float>> data);
Tree* BestGen(vector<Tree*> population,vector<vector<float>> data);


int main(int argc, char const *argv[]) {
    string str;
    int i =1;
    while(i<argc){
        str.append(argv[i]);
        str.append(" ");
        i++;
    }
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia(str);
    
    srand(inputConfig->seed);
    
    vector<Tree*> population = initialPopulation(inputConfig->populationSize);
 
    Tree* solution = evoluition(inputConfig->numGeneration,population,inputConfig->datasetTrain);

    std::cout<<"----------------------------"<<endl;
    solution->print();
    std::cout<<"Fitness Treino: "<<fitness(solution,inputConfig->datasetTrain)<<endl;
    std::cout<<"Fitness Teste: "<<fitness(solution,inputConfig->datasetTest)<<endl;
    std::cout<<"----------------------------"<<endl;
    return 0;
}
Tree* evoluition(int generation,vector<Tree*>population,vector<vector<float>> data){
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    int it =0;
    vector<Tree*>newPopulation = population;
    vector<Tree*>selected;
    Tree* best = nullptr;
    while(it<generation){
        best = BestGen(newPopulation,data);
        std::cout<<"Gen "<<it<<":"<<fitness(best,data)<<endl;
        best->print();
        selected = selection(inputConfig->selectionType,newPopulation,data);
        newPopulation = operatorSelector(selected,inputConfig->mutationProb,inputConfig->crossoverProb);
        for(int i = 0; i <inputConfig->elitism;i++){
            newPopulation.push_back(best->copy());
        }
        it++;
    }
    best = BestGen(newPopulation,data);
    return best;
}
Tree* BestGen(vector<Tree*> population,vector<vector<float>> data){
    Tree* best = nullptr;
    set<float> diversity;
    float aux;
    for(int i =0; i<population.size();i++){
        if(best == nullptr){
            best= population[i]->copy();
        }
        aux = fitness(population[i],data);
        diversity.insert(aux);
        if(aux<fitness(best,data)){
            best = population[i]->copy();
        }
    }
    std::cout<<"Diversidade: "<<diversity.size()<<endl; 
    return best;
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
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    vector<string> terminal = inputConfig->terminals;
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
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    vector<string> terminal = inputConfig->terminals;
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
    float random;

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

        it+=1;

    }
    return selected;
}

float fitness(Tree* subject,vector<vector<float>>& dataset){
    int N = dataset.size();
    float sumEval =0;
   for(int i=0;i<N;i++){
        sumEval += pow(eval(subject,dataset[i])-dataset[i].back(),2);
   }
   float RSME = sumEval/N;
    return sqrt(RSME);
}

float eval(Tree* subject,vector<float> input){
    return subject->f(input);
}

vector<Tree*> selectionRoulette(vector<Tree*> population,vector<vector<float>>& input){
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    vector<Tree*> selected;
     vector<pair<float,Tree*>> roulette;
    float fitnessTotal =0;
    float fitnessInd=0;
    for(int i=0;i<population.size();i++){
        fitnessInd = fitness(population[i],input);
        if(fitnessInd < pow(10,5)){ // filtra individuos com a fitness muito alta
            roulette.push_back(make_pair(fitnessInd,population[i]));
            fitnessTotal += fitnessInd;
        }
    }
    std::sort(roulette.begin(),roulette.end());
    for(int i = 0;i<roulette.size();i++){
        roulette[i].first =  1-(roulette[i].first/fitnessTotal);
        
    }
    float random;
    int it=0;
    while(it<population.size()-inputConfig->elitism){
        random = (rand()%10000000)/10000000;
        int j =0;
        while(random>roulette[j].first){
            j++;
        }
        selected.push_back(roulette[j].second->copy());
        it++;
    }
    return selected;
}

Tree* selectionTournament(vector<Tree*> population,vector<vector<float>>& input,int k){
    Tree* bestTree = nullptr;
    int random=0;
    for(int i=0;i<k;i++){
        random = rand()%population.size();
        if (bestTree == nullptr){
            bestTree = population[random];
        }
        if(fitness(bestTree,input)>fitness(population[random],input)){
            bestTree = population[random];
        }
    }
    return bestTree->copy();
}


vector<Tree*> selection(int type,vector<Tree*> population,vector<vector<float>>& input){
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    vector<Tree*> selected;
        switch(type){
            case 0 : 
                selected = selectionRoulette(population,input);
                break;
            case 1 : 
                while(selected.size()<population.size()-inputConfig->elitism){
                    selected.push_back(selectionTournament(population,input,inputConfig->k));
                }
                break;
            case 2 : 
                break;
        }
    
    return selected;
}