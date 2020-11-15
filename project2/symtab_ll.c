#include "symtab.h"

void entry_init(entry *self, char *key, VAL_T value){
    sprintf(self->key, "%s", key);
    self->value = value;
}

/*
 * symbol table type, linked list impl
 */
struct symtab {
    entry entry;
    struct symtab *next;
};

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab *symtab_init(){
    symtab *self = malloc(sizeof(symtab));
    memset(self, '\0', sizeof(symtab));
    self->next = NULL;
    return self;
}

int symtab_insert(symtab *self, char *key, VAL_T value){
    symtab *ptr = self;
    while(ptr->next != NULL){
        ptr = ptr->next;
        // printf("current key is %s**********\n", ptr->entry.key);
        if(strcmp(ptr->entry.key, key) == 0){
            printf("table insert \"%s fail!!!!!!!!!\n", key);
            return 0;
        }
    }
    symtab *node = malloc(sizeof(symtab));
    memset(node, '\0', sizeof(symtab));
    entry_init(&node->entry, key, value);
    node->next = NULL;
    ptr->next = node;
    printf("table insert \"%s\" success, ", key);
    return 1;
}

VAL_T symtab_lookup(symtab *self, char *key){
    // printf("enter symtab lookup key %s\n", key);
    symtab *ptr = self->next;
    while(ptr != NULL){
        // printf("\t table current key is %s\n", ptr->entry.key);
        if(strcmp(ptr->entry.key, key) == 0)
            return ptr->entry.value;
        ptr = ptr->next;
    }
    return NULL;
}

int symtab_remove(symtab *self, char *key){
    symtab *ptr = self, *tmp;
    while(ptr->next != NULL) {
        if(strcmp(ptr->next->entry.key, key) == 0){
            tmp = ptr->next;
            ptr->next = ptr->next->next;
            free(tmp);
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}


int symtab_size(symtab *self){
    symtab *ptr = self;
    int size = 0;
    while(ptr->next != NULL) {
        size++;
        ptr = ptr->next;
    }
    return size;
}

void symtab_print(symtab *table){
    // printf("enter table print\n");
    symtab* p;
    p = table;
    int i = 0;
    while (p->next != NULL) {
        p = p->next;
        printf("entry%d: {%s: ", i, p->entry.key);
        IDType_print(p->entry.value);
        printf("}\n");
        i++;
    }
}