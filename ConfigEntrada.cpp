#include "ConfigEntrada.hpp"
ConfigEntrada* ConfigEntrada::instancia = nullptr;

ConfigEntrada* ConfigEntrada::getInstancia(){
    return instancia;
}

ConfigEntrada* ConfigEntrada::getInstancia(string entrada){
    if(instancia==nullptr){
        instancia = new ConfigEntrada(entrada);
    }
    return getInstancia();
}

ConfigEntrada::ConfigEntrada(string entrada)
{
    // p s k m c
    this->numGeneration = stoi(identificaFlag(entrada,"-g "));
    this->populationSize = stoi(identificaFlag(entrada,"-p "));
    this->selectionType = stoi(identificaFlag(entrada,"-st "));
    this->seed = stoi(identificaFlag(entrada,"-s "));
    this->k = stoi(identificaFlag(entrada,"-k "));
    this->mutationProb = stoi(identificaFlag(entrada,"-m "));
    this->crossoverProb = stoi(identificaFlag(entrada,"-c "));
    this->trainData = identificaFlag(entrada,"-tr ");
    this->testData = identificaFlag(entrada,"-ts ");
    this->instancia = this;
    this->datasetTest = createVectors(this->testData);
    this->datasetTrain = createVectors(this->trainData);
    this->numVariables = this->datasetTrain[0].size()-1;
    generateVariables();

}


string ConfigEntrada::identificaFlag(string str,string flag){
     size_t pos = str.find(flag);
     size_t aux ;
    if(pos != string::npos){
        pos+=flag.size();
        aux = str.find(" ",pos);
        return str.substr(pos,aux-pos);
    }
    else{
        return "0";
    }
}

 vector<vector<float>> ConfigEntrada::createVectors(string Dataset){
    string line;
    vector<float> aux;
    vector<vector<float>> dataset;
     std::ifstream file(Dataset);

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)){
            
            aux = parseCSVLine(line);
            dataset.push_back(aux);
        }
    }
    return dataset;

 }
 vector<float> ConfigEntrada::parseCSVLine(string& line) {
    std::vector<float> values;
    std::istringstream iss(line);
    std::string token;
    
    while (std::getline(iss, token, ',')) {
            float value = std::stof(token);
            values.push_back(value);
            
    }
    
    return values;
}

void ConfigEntrada::generateVariables(){
    string aux = "x";
    for(int i=0; i<this->numVariables; i++){
        this->terminals.push_back(aux+to_string(i));
    }
}