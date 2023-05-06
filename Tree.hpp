#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "Node.hpp"


using namespace std;


class Tree
{
private:
    bool mutation(Node* node);
    int dephtTree(Node* node);

public:
    Node* root;
    Tree(Node* root);
    int dephtTree();
    void mutation();
    float fitness(float input);
    static void updateDepth(Node* node,int depth);
    void print();
};

#endif