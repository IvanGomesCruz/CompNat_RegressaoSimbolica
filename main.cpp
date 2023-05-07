#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include <random>
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
Tree* selectionLexicase(vector<Tree*> population,vector<vector<float>> input);
float calcMedian(std::vector<float>& vector);
float threshold(vector<Tree*> selected,vector<float> input);
float fitness(Tree* subject,vector<vector<float>>& dataset);
vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover);
vector<Tree*> initialPopulation(int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);
float eval(Tree* subject,vector<float> input);
Tree* evoluition(int generation,vector<Tree*>population,vector<vector<float>> data);
Tree* BestGen(vector<Tree*> population,vector<vector<float>> data);
Tree* GP(int seed);


int main(int argc, char const *argv[]) {
    string str;
    int i =1;
    while(i<argc){
        str.append(argv[i]);
        str.append(" ");
        i++;
    }
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia(str);
    
    Tree* solution =  GP(inputConfig->seed);
    vector<Tree*> random = initialPopulation(inputConfig->numGeneration*inputConfig->populationSize);
    Tree* bestRandom = BestGen(random,inputConfig->datasetTrain);

    std::cout<<"----------------------------"<<endl;
    std::cout<<"GP:"<<endl;
    solution->print();
    std::cout<<"Fitness Treino: "<<fitness(solution,inputConfig->datasetTrain)<<endl;
    std::cout<<"Fitness Teste: "<<fitness(solution,inputConfig->datasetTest)<<endl;
    std::cout<<"Random:"<<endl;
    bestRandom->print();
    std::cout<<"Fitness Treino: "<<fitness(bestRandom,inputConfig->datasetTrain)<<endl;
    std::cout<<"Fitness Teste: "<<fitness(bestRandom,inputConfig->datasetTest)<<endl;
    std::cout<<"----------------------------"<<endl;
    return 0;
}
Tree* GP(int seed){
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    
    srand(inputConfig->seed);
    
    vector<Tree*> population = initialPopulation(inputConfig->populationSize);
    Tree* solution = evoluition(inputConfig->numGeneration,population,inputConfig->datasetTrain);
    return solution;
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
                while(selected.size()<population.size()-inputConfig->elitism){
                    selected.push_back(selectionLexicase(population,input));
                }

                break;
        }
    
    return selected;
}

float threshold(vector<Tree*> selected,vector<float> input){
    
    float median;
    vector<float> fitnessExemples;
    float e_min; 
    float MAD;
    float epsilon;
    vector<vector<float>> aux;
    aux.push_back(input);
     for(int i = 0; i <selected.size(); i++){
            fitnessExemples.push_back(fitness(selected[i],aux));
        }
        std::sort(fitnessExemples.begin(), fitnessExemples.end());
        e_min = fitnessExemples[0];
        median = calcMedian(fitnessExemples);
        for(int i = 0; i <fitnessExemples.size(); i++){
            fitnessExemples[i]= abs(fitnessExemples[i] - median);
        }
        MAD = calcMedian(fitnessExemples);
        return e_min + MAD;
}

Tree* selectionLexicase(vector<Tree*> population,vector<vector<float>> input){
    ConfigEntrada* inputConfig = ConfigEntrada::getInstancia();
    vector<float> inputExemple;
    vector<Tree*> selected = population;
    vector<Tree*> next;
    vector<vector<float>> aux;
    std::mt19937 generator(rand());
    float valueThreshold;
    std::shuffle(input.begin(), input.end(), generator);
    for(int it =0; it < input.size(); it++){
        if(selected.size() == 1){
            break;
        }
        aux.clear();
        inputExemple = input.back();
        aux.push_back(inputExemple);
        input.pop_back();
        valueThreshold =threshold(selected,inputExemple);
        for(int i = 0; i<selected.size(); i++){
            if(fitness(selected[i],aux)<valueThreshold){
                next.push_back(selected[i]);
            }
        }
        if(next.size()==0){
            next.push_back(selected[rand()%selected.size()]);
        }
        selected.clear();
        selected = next;
        next.clear();

    }
    return selected[0];
}

float calcMedian(std::vector<float>& vector) {
    std::sort(vector.begin(), vector.end());

    if (vector.size() % 2 == 0) {
        // Tamanho par
        int ind1 = vector.size() / 2 - 1;
        int ind2 = vector.size() / 2;
        return (vector[ind1] + vector[ind2]) / 2.0f;
    } else {
        // Tamanho ímpar
        int ind = vector.size() / 2;
        return vector[ind];
    }
}