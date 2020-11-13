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

void push(Stack* s) {
    Stack* cell;
    cell = (Stack*)malloc(sizeof(Stack));
    if (!cell) {
        printf("空间不足\n");
    }
    memset(cell->data,0,sizeof(int)*26);
    cell->next = s->next;
    s->next = cell;
}

int* pop(Stack* s) {
    Stack* firstCell;
    int* topData;
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

int* peek(Stack* s){
    if(is_empty(s)){
        printf("空栈\n");
        return NULL;
    }
    Stack* firstCell = s->next;
    return firstCell->data;
}

// void stack_print(Stack* s) {
//     Stack* p;
//     p = s;
//     int i = 0;
//     while (p->next != NULL) {
//         p = p->next;
//         printf("stack%d:\n", i);
//         // printf("in print stack, table is null?, %d\n", p->data == NULL);
//         symtab_print(p->data);
//         i++;
//     }
// }