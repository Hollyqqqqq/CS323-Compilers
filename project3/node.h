#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct Node {
    char* name;
    char* text;
    int lineNum;
    int childrenNum;
    int isTerminal;
    int needDetail;
    struct Node *children[10];
}Node;

Node* createTerminal(char* name, char* text, int lineNum, int needDetail);

Node* createNonTerminal(char* name);

void buildTree(Node* father, int childrenNum, ...);

void printTree(Node* root, int depth, FILE* fp);

#endif