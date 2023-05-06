#ifndef CONFIGENTRADA_H
#define CONFIGENTRADA_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
class ConfigEntrada
{
private:
    ConfigEntrada(string entrada);
    static ConfigEntrada* instancia;
    string identificaFlag(string entrada,string flag);
    string trainData;
    string testData;
    vector<vector<float>> createVectors(string Dataset);
    vector<float> parseCSVLine(string& line);
    void generateVariables();
public:
    vector<string> terminals;
    vector<string> operators;
    int seed;
    int numGeneration;
    int numVariables;
    int selectionType;
    int populationSize;
    float crossoverProb;
    float mutationProb;
    vector<vector<float>> datasetTrain;
    vector<vector<float>> datasetTest;


    int k;
    static ConfigEntrada* getInstancia();
    static ConfigEntrada* getInstancia(string entrada);
};



#endif