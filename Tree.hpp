#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "Node.hpp"
#include <queue>


using namespace std;


class Tree
{
private:
    bool mutation(Node* node);
    vector<Node*> BFS(int depth);
    int dephtTree(Node* node);
public:
    int MaxDepth;
    Node* root;
    Tree(Node* root,int maxDepht);
    int dephtTree();
    void mutation();
    float f(float input);
    static void updateDepth(Node* node,int depth);
    void crossover(Tree* partiner);
    void print();
};

#endif