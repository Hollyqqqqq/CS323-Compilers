#include "symtab.h"
/*
 * symbol table type, binary tree impl
 */
struct symtab {
    entry entry;
    struct symtab *left, *right;
};

int head = 0;
int tail = 0;//tail的位置没有元素
symtab *queue[1000];

void resetQueue(){
    head = 0;
    tail = 0;
}

void enQueue(symtab *node){
    queue[tail] = node;
    tail++;
}

symtab *deQueue(){
    symtab * node = queue[head];
    head++;
    return node;
}

int queueEmpty(){
    if (head < tail){
        return 0;
    }
    return 1;
}

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab *symtab_init(){
    symtab *self = malloc(sizeof(symtab));
    memset(self, '\0', sizeof(symtab));
    self->left = NULL;
    self->right = NULL;
    return self;
}

int symtab_insert_node(symtab *self, symtab *node){
    symtab *ptr = self;
    symtab *targetPos;
    int find = 0;
    resetQueue();
    enQueue(ptr);
    while (!queueEmpty()){
        ptr = deQueue();
        if(strcmp(ptr->entry.key, node->entry.key) == 0){
            return 0;
        }
        if (ptr->left == NULL || ptr->right == NULL){
            if(find == 0){
                find = 1;
                targetPos = ptr;
            }
        }
        if(ptr->left != NULL) {
            enQueue(ptr->left);
        }
        if(ptr->right != NULL) {
            enQueue(ptr->right);
        }
    }
    if (targetPos->left == NULL){
        targetPos->left = node;
    } else{
        targetPos->right = node;
    }
    return 1;
}

int symtab_insert(symtab *self, char *key, VAL_T value){
    symtab *node = malloc(sizeof(symtab));
    memset(node, '\0', sizeof(symtab));
    entry_init(&node->entry, key, value);
    node->left = NULL;
    node->right = NULL;
    int result = symtab_insert_node(self, node);
    if (result == 0){
        free(node);
    }
    return result;
}

symtab *findParent(symtab *self, char *key){
    resetQueue();
    symtab *ptr = self;
    if (ptr != NULL){enQueue(ptr);}
    while (!queueEmpty()){
        ptr = deQueue();
        if (ptr->left != NULL){
            if (strcmp(ptr->left->entry.key, key) == 0){
                return ptr;
            }
            enQueue(ptr->left);
        }
        if (ptr->right != NULL){
            if (strcmp(ptr->right->entry.key, key) == 0){
                return ptr;
            }
            enQueue(ptr->right);
        }
    }
    return NULL;
}

VAL_T symtab_lookup(symtab *self, char *key){
    resetQueue();
    symtab *ptr = self;
    if (ptr != NULL){enQueue(ptr);}
    while (!queueEmpty()){
        ptr = deQueue();
        if (strcmp(ptr->entry.key, key) == 0){
            return ptr->entry.value;
        }
        if (ptr->left != NULL){
            enQueue(ptr->left);
        }
        if (ptr->right != NULL){
            enQueue(ptr->right);
        }
    }
    return -1;
}

int symtab_remove(symtab *self, char *key){
    symtab *parent = findParent(self, key);
    symtab *target;
    if (parent != NULL){
        if (parent->left != NULL && strcmp(parent->left->entry.key, key) == 0){
            target = parent->left;
            parent->left = NULL;
        } else{
            target = parent->right;
            parent->right = NULL;
        }
        if (target->left != NULL){
            symtab_insert_node(self, target->left);
            target->left = NULL;
        }
        if (target->right != NULL){
            symtab_insert_node(self, target->right);
            target->right = NULL;
        }
        free(target);
        return 1;
    }
    return 0;
}