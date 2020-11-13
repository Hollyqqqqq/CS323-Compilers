#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR NULL

typedef struct Stack {
    int data[26];
    struct Stack* next;
}Stack;

Stack* stack_init();

int is_empty(Stack*);

void push(Stack*);

int* pop(Stack*);

int* peek(Stack*);

// void stack_print(Stack*);

#endif