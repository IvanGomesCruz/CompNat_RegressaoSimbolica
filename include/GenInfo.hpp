#ifndef GENINFO_H
#define GENINFO_H
#include "Tree.hpp"
#include <iostream>
struct GenInfo
{
    int generation;
    int diversity;
    Tree* worse;
    Tree* best;
    double average;
    double depthAverage;
    
};



#endif