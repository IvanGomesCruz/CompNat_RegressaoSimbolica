#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <set>
#include <iomanip> 
#include <random>
#include <cstring>
#include "Node.hpp"
#include "NodeOperator.hpp"
#include "NodeTerminal.hpp"
#include "Tree.hpp"
#include "InputConfig.hpp"
#include "GenInfo.hpp"
#define DEPTHMAX 7
#define POPULATIONSIZE 10
int numMutations =0;
int numCrossover =0;
int improvementMutations =0;
int improvementCrossover =0;

const vector<string> operators = {"+","-","*","/"};
//const vector<string> terminal = {"x","1","2","3"};

using namespace std;
//----------------------------------------------------------------
// Selection functions
vector<Tree*> selection(int type,vector<Tree*> population,vector<vector<float>>& input);
vector<Tree*> selectionRoulette(vector<Tree*> population,vector<vector<float>>& input);
Tree* selectionTournament(vector<Tree*> population,vector<vector<float>>& input,int k);
Tree* selectionLexicase(vector<Tree*> population,vector<vector<float>> input);
float threshold(vector<Tree*> selected,vector<float> input);
float calcMedian(std::vector<float> vector);
float MAD(vector<float> e);
//----------------------------------------------------------------
//----------------------------------------------------------------

//Population generation functions
vector<Tree*> initialPopulation(unsigned long int size);
Node* generateFullRandomTree(int size,int depthmax = DEPTHMAX);
Node* generateGrowRandomTree(int size,int depthmax = DEPTHMAX);
//----------------------------------------------------------------
//----------------------------------------------------------------
//Run algorithm founctions
vector<GenInfo> GP(int seed);
vector<GenInfo> evolution(int generation,vector<Tree*>population,vector<vector<float>> data);
vector<Tree*> operatorSelector(vector<Tree*>selected,float probMutation,float probCrossover);
float fitness(Tree* subject,vector<vector<float>>& dataset);
float eval(Tree* subject,vector<float> input);
//----------------------------------------------------------------
GenInfo getGenInfo (vector<Tree*> population,vector<vector<float>> data, int generation);
void generateLogs(vector<GenInfo>& info, int seed);
float lower(float op1, float op2);
float calcMean(vector<float> values);
float standardDeviation(vector<float> values); 

int main(int argc, char const *argv[]) {
    string str;
    int i =1;
    while(i<argc){
        str.append(argv[i]);
        str.append(" ");
        i++;
    }
    InputConfig* inputConfig = InputConfig::getInstancia(str);
    vector<Tree*> bests;
    
    for(int seed = 0; seed<30;seed++){
        inputConfig->seed = seed;
        srand(inputConfig->seed);
        vector<GenInfo> evolution =  GP(inputConfig->seed);
        generateLogs(evolution,inputConfig->seed);
        bests.push_back(evolution.back().best);
    }
    Tree* best = nullptr;
    float bestFitness =INFINITY;
    int seedBest;
    float thisFitness;
    vector<float> bestsFitness;
    for(unsigned long int it =0 ;it<bests.size();it++){
        thisFitness = fitness(bests[it],inputConfig->datasetTest);
        bestsFitness.push_back(thisFitness);
        if(thisFitness<bestFitness){
            best = bests[it];
            bestFitness = thisFitness;
            seedBest = it;
        }
    }

    std::ofstream outputFile1;
    str = inputConfig->trainData;
    size_t pos = str.find("/");
    pos = str.find("/",pos+1);
    string directory = str.substr(0,pos);
    
    string name = directory+"/results/p"+to_string((int)inputConfig->populationSize);
    name+="g"+to_string((int)inputConfig->numGeneration)+"m"+to_string((int)(inputConfig->mutationProb))+"c"+to_string((int)(inputConfig->crossoverProb))+"e"+to_string(inputConfig->elitism);
    name += "st" + to_string(inputConfig->selectionType);
    outputFile1.open(name+".txt");

    //Tree* bestRandom = BestGen(random,inputConfig->datasetTrain);
    string selection;
    switch(inputConfig->selectionType){
        case 0 : selection = "Roleta";break;
        case 1 : selection = "Torneio";break;
        case 2 : selection = "Lexicase";break;
    }
    outputFile1<<"Tamanho da população: "<< inputConfig->populationSize<<endl;
    outputFile1<<"Número de gerações: "<<inputConfig->numGeneration<<endl;
    outputFile1<<"Tipo de seleção: "<<selection<<endl;
    outputFile1<<"Probabilidade de mutação: "<<inputConfig->mutationProb<<endl;
    outputFile1<<"Probabilidade de crossover: "<<inputConfig->crossoverProb<<endl;
    outputFile1<<"----------------------------"<<endl;
    outputFile1<<"Melhorias em mutações:"<<std::setprecision(3)<<improvementMutations*100.00/numMutations<<"%"<<endl;
    outputFile1<<"Melhorias em crossover: "<<std::setprecision(3)<<improvementCrossover*100.00/numCrossover<<"%"<<endl;
    outputFile1<<"Desvio padrão dos resultados:"<<standardDeviation(bestsFitness)<<endl;
    outputFile1<<"----------------------------"<<endl;
    outputFile1<<"Melhor resultado:"<<endl;
    outputFile1<<"Seed: "<<seedBest<<endl;
    outputFile1<<best->print()<<endl;
    outputFile1<<"Fitness Treino: "<<fitness(best,inputConfig->datasetTrain)<<endl;
    outputFile1<<"Fitness Teste: "<<fitness(best,inputConfig->datasetTest)<<endl;
    return 0;
}


void generateLogs(vector<GenInfo>& info, int seed){
    InputConfig* inputConfig = InputConfig::getInstancia();
    string str = inputConfig->trainData;
    size_t pos = str.find("/");
    pos = str.find("/",pos+1);
    string directory = str.substr(0,pos);
    
    std::ofstream outputFile1;
    string name = directory+"/logs/s"+to_string(seed)+"p"+to_string((int)inputConfig->populationSize);
    name+="g"+to_string((int)inputConfig->numGeneration)+"m"+to_string((int)(inputConfig->mutationProb))+"c"+to_string((int)(inputConfig->crossoverProb))+"e"+to_string(inputConfig->elitism);
    name += "st" + to_string(inputConfig->selectionType);
    outputFile1.open(name+"_generationsResults.csv"); 
    std::ofstream outputFile2;
    outputFile2.open(name+"_generationsDiversity.csv"); 
    std::ofstream outputFile3;
    outputFile3.open(name+"_generationBloat.csv"); 
    double best,worse,average,diversity,depthAverage;
    if (outputFile1.is_open() && outputFile2.is_open()) { 
        outputFile1<<"Generation"<<","<<"Best"<<","<<"Worse"<<","<<"Average"<<"\n"; 
        outputFile2<<"Generation"<<","<<"Diversity"<<"\n"; 
        outputFile3<<"Gerneration"<<","<<"Depth average"<<"\n"; 
        info.erase(info.begin());
        for(auto it :info){
           
                best = fitness(it.best,inputConfig->datasetTrain);
                worse = fitness(it.worse,inputConfig->datasetTrain);
                outputFile1<<it.generation<<","<<best<<","<<worse<<","<<average<<"\n"; 
                average = it.average;
                diversity = it.diversity;
                outputFile2<<it.generation<<","<<diversity<<"\n"; 
                depthAverage = it.depthAverage;
                outputFile3<<it.generation<<","<<depthAverage<<"\n"; 

        }
        outputFile1.close(); 
        outputFile2.close(); 
        //std::cout << "Os dados foram gravados nos arquivos de saída.\n";
    } else {
        std::cout << "Não foi possível abrir os arquivos de saída.\n";
    }

}

vector<GenInfo> GP(int seed){
    InputConfig* inputConfig = InputConfig::getInstancia();
    
    vector<Tree*> population = initialPopulation(inputConfig->populationSize);
    vector<GenInfo> infos = evolution(inputConfig->numGeneration,population,inputConfig->datasetTrain);
    return infos;
}
vector<GenInfo> evolution(int generation,vector<Tree*>population,vector<vector<float>> data){
    InputConfig* inputConfig = InputConfig::getInstancia();
    int it =0;
    vector<Tree*>newPopulation = population;
    vector<Tree*>selected;
    vector<GenInfo> infos;
    while(it<generation){
        infos.push_back(getGenInfo(newPopulation,data,it));
        selected = selection(inputConfig->selectionType,newPopulation,data);
        newPopulation = operatorSelector(selected,inputConfig->mutationProb,inputConfig->crossoverProb);
        for(int i = 0; i <inputConfig->elitism;i++){
            newPopulation.push_back(infos.back().best->copy());
        }
        it++;
    }
    return infos;
}

GenInfo getGenInfo (vector<Tree*> population,vector<vector<float>> data, int generation){
    Tree* best = nullptr;
    Tree* worse = nullptr;
    set<float> diversity;
    GenInfo genInfo;
    genInfo.generation = generation;
    double aux;
    double average =0;
    double depthAverage=0;
    for(auto it : population){
        if(best == nullptr&&worse == nullptr){
            best = it->copy();
            worse = best;
        }
        aux = fitness(it,data);
        average +=aux;
        diversity.insert(aux);
        it->updateDepth(it->root,0);
        depthAverage += it->dephtTree();

        if(aux<fitness(best,data)){
            best = it->copy();
        }
        if(aux>fitness(worse,data)){
            worse = it->copy();
        }

    }
    average /=population.size();
    depthAverage /= population.size();
    genInfo.average = average;
    genInfo.depthAverage = depthAverage;
    genInfo.best = best;
    genInfo.worse = worse;
    genInfo.diversity = diversity.size();
    return genInfo;
}


vector<Tree*> initialPopulation(unsigned long int size){
    vector<Tree*> population;
    Tree* tree;
    while(population.size()<=size){
        if(population.size() <=size/2){
            
            tree = new Tree(generateFullRandomTree(0,population.size()%DEPTHMAX+1),DEPTHMAX);
            
        }
        else{
            tree = new Tree(generateGrowRandomTree(0,population.size()%DEPTHMAX+1),DEPTHMAX);
        }
            tree->updateDepth(tree->root,0);
            population.push_back(tree);
    }

    return population;
}

Node* generateFullRandomTree(int depth,int depthmax) {
    int random;
    InputConfig* inputConfig = InputConfig::getInstancia();
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
    InputConfig* inputConfig = InputConfig::getInstancia();
    vector<string> terminal = inputConfig->terminals;
    int random;
    random = rand()%2; //Determina se será Operador ou Terminal
    if((random ==1 || depth == DEPTHMAX)&&depth!=0){
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
    InputConfig* inputConfig = InputConfig::getInstancia();
    unsigned long int it=0;
    float random;
    float fitnessParent1,fitnessParent2;
    while(it<selected.size()){

        random = rand() % 100;
        if(random<probMutation){

            fitnessParent1 = fitness(selected[it],inputConfig->datasetTrain);
            selected[it]->mutation();
            if(fitness(selected[it],inputConfig->datasetTrain)<fitnessParent1){
                improvementMutations++;
            }
            numMutations++;

            it++;

        }
        else if(random>= probMutation && random<probMutation+probCrossover && it+1<selected.size()){ //A ultima condição é para o caso em que it esteja na ultima posição

            numCrossover++;
            fitnessParent1 = fitness(selected[it],inputConfig->datasetTrain);
            fitnessParent2 = fitness(selected[it+1],inputConfig->datasetTrain);
            selected[it]->crossover(selected[it+1]);
            if(fitness(selected[it],inputConfig->datasetTrain)<lower(fitnessParent1,fitnessParent2)||fitness(selected[it+1],inputConfig->datasetTrain)<lower(fitnessParent1,fitnessParent2)){
                improvementCrossover++;
            }
            it+=2;

        }
        else{
            it++;
        }
    }
    return selected;
}
float lower(float op1, float op2){
    if(op1<op2) return op1;
    return op2;
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
    InputConfig* inputConfig = InputConfig::getInstancia();
    vector<Tree*> selected;
     vector<pair<float,Tree*>> roulette;
    float fitnessTotal =0;
    float fitnessInd=0;
    for(auto it:population){
        fitnessInd = fitness(it,input);
        if(fitnessInd < pow(10,5)){ // filtra individuos com a fitness muito alta
            roulette.push_back(make_pair(fitnessInd,it));
            fitnessTotal += fitnessInd;
        }
    }
    std::sort(roulette.begin(),roulette.end());
    for(auto it : roulette){
        it.first =  1-(it.first/fitnessTotal);
        
    }
    float random;
    unsigned long int it=0;
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
    InputConfig* inputConfig = InputConfig::getInstancia();
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
    for(auto aux : population){
        delete aux;
    }
    return selected;
}

Tree* selectionLexicase(vector<Tree*> population,vector<vector<float>> input){
    vector<float> fitness;
    vector<Tree*> selected = population;
    float mad;
    int it =0;
    float error;
    float minError = INFINITY;
    int idxMinError =0;
    int random;
    while(selected.size() >1 && input.size()>0){
        random = rand()%input.size();

        for(auto aux=0;aux<selected.size();aux++){

            error = selected[aux]->f(input[random])-input[random].back();
            if(minError < error){
                minError = error;
                idxMinError = aux;
            }
            fitness.push_back(error);
        }
        input.erase(input.begin() + random);
        mad = MAD(fitness);
        for(auto aux =0; aux<selected.size();aux++){
            if(fitness[aux]>fitness[idxMinError]+mad){
                selected.erase(selected.begin()+aux);
             }
        }
        it++;
        fitness.clear();
    }
    return selected[0];
}
float MAD(vector<float> e){
    float e_median = calcMedian(e);
    for(auto e_k:e){
        e_k = abs(e_k-e_median);
    }
    return calcMedian(e);
}

float calcMedian(std::vector<float> vector) {
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

float calcMean(vector<float> values) {
    float sum = 0;
    for (float it : values) {
        sum += it;
    }
    return sum / values.size();
}

float standardDeviation(vector<float> values) {
    float mean = calcMean(values);
    float sumOfSquares = 0;

    for (double it : values) {
        sumOfSquares += std::pow(it - mean, 2);
    }

    double variance = sumOfSquares / values.size();
    double standardDeviation = sqrt(variance);

    return standardDeviation;
}