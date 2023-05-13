#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
class InputConfig
{
private:
    InputConfig(string entrada);
    static InputConfig* instancia;
    string identificaFlag(string entrada,string flag);
    string testData;
    vector<vector<float>> createVectors(string Dataset);
    vector<float> parseCSVLine(string& line);
    void generateVariables();
public:
    string trainData;
    vector<string> terminals;
    vector<string> operators;
    int seed;
    int numGeneration;
    int numVariables;
    int selectionType;
    int populationSize;
    int dephtMax;
    int elitism;
    float crossoverProb;
    float mutationProb;
    vector<vector<float>> datasetTrain;
    vector<vector<float>> datasetTest;


    int k;
    static InputConfig* getInstancia();
    static InputConfig* getInstancia(string entrada);
};



#endif