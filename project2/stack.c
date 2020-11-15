#include "stack.h"

Stack* stack_init(){
    Stack* s;
    s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        printf("空间不足\n");
    }
    s->next = NULL;
    return s;
}

int is_empty(Stack* s){
    return (s->next == NULL);
}

void push(Stack* s, ElementType data) {
    Stack* cell;
    cell = (Stack*)malloc(sizeof(Stack));
    if (!cell) {
        printf("空间不足\n");
    }
    cell->data = data;
    cell->next = s->next;
    s->next = cell;
}

ElementType pop(Stack* s) {
    Stack* firstCell;
    ElementType topData;
    if (is_empty(s)) {
        printf("空栈\n");
        return ERROR;
    }
    firstCell = s->next;
    s->next = firstCell->next;
    topData = firstCell->data;
    free(firstCell);
    return topData;
}

ElementType peek(Stack* s){
    if(is_empty(s)){
        printf("空栈\n");
        return NULL;
    }
    Stack* firstCell = s->next;
    return firstCell->data;
}

VAL_T stack_lookup(Stack *stack, char *key){
    Stack *curr_stack = stack->next;
    while (curr_stack != NULL)
    {
        VAL_T id = symtab_lookup(curr_stack->data, key);
        if (id != NULL)
        {
            return id;
        }
        curr_stack = curr_stack->next;
    }
    return NULL;
}

void stack_print(Stack* s) {
    Stack* p;
    p = s;
    int i = 0;
    while (p->next != NULL) {
        p = p->next;
        printf("stack%d:\n", i);
        // printf("in print stack, table is null?, %d\n", p->data == NULL);
        symtab_print(p->data);
        i++;
    }
}