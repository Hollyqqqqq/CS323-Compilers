#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"

#define ElementType symtab*
#define ERROR NULL

typedef struct Stack {
    ElementType data;
    struct Stack* next;
}Stack;

Stack* stack_init();

int is_empty(Stack*);

void push(Stack*, ElementType);

ElementType pop(Stack*);

ElementType peek(Stack*);

VAL_T stack_lookup(Stack*, char*);

void stack_print(Stack*);

#endif