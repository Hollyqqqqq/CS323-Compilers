#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Node {
    char* name;
    char* text;
    int lineNum;
    int childrenNum;
    int isTerminal;
    int needDetail;
    struct Node *children[10];
}Node;

Node* createTerminal(char* name, char* text, int lineNum, int needDetail){
    Node* node = (Node *) malloc(sizeof(Node));
    node->name = name;
    if(needDetail){
        node->needDetail = 1;
        node->text = (char*)malloc(sizeof(char)*strlen(text));
        strcpy(node->text,text);
    }
    node->lineNum = lineNum;
    node->childrenNum = 0;
    node->isTerminal = 1;
    return node;
}

Node* createNonTerminal(char* name){
    Node* node = (Node *) malloc(sizeof(Node));
    node->name = name;
    node->text = "";
    node->isTerminal = 0;
    node->needDetail = 0;
    return node;
}

void buildTree(Node* father, int childrenNum, ...){
    va_list arg_ptr;
    va_start(arg_ptr, childrenNum);
    for(int i = 0; i < childrenNum; i++) {
        father->children[i] = va_arg(arg_ptr, Node*);
    }
    if (childrenNum > 0){
        father->lineNum = father->children[0]->lineNum;
        father->childrenNum = childrenNum;
    }
}

//if nonterminal node has empty children, do not print.
void printTree(Node* root, int depth, FILE* fp){
    if (!root->isTerminal && root->childrenNum == 0){
        return;
    }

    for (int i = 0; i < depth; ++i) {
        fprintf(fp, "  ");
    }
    if(!root->isTerminal) {
        fprintf(fp, "%s (%d)\n", root->name, root->lineNum);
    } else{
        if (root->needDetail){
            fprintf(fp, "%s: %s\n", root->name, root->text);
        }else{
            fprintf(fp, "%s\n", root->name);
        }
    }

    for (int j = 0; j < root->childrenNum; ++j) {
        printTree(root->children[j], depth+1, fp);
    }
}