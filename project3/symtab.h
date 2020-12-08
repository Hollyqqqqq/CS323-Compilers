#ifndef SYMTAB_H
#define SYMTAB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"idtype.h"

#define KEY_LEN 32
#define VAL_T IDType*

typedef struct symtab symtab;

/* symbol table entry, only used internally */
typedef struct entry {
    char key[KEY_LEN+1];
    VAL_T value;
} entry;

void entry_init(entry *self, char *key, VAL_T value);


// init a single symbol table
symtab *symtab_init();

// insert a key-value pair to the table
// if insert success, return 1, otherwise 0
int symtab_insert(symtab*, char*, VAL_T);

// lookup the value of a specific key
// return -1 if not found
VAL_T symtab_lookup(symtab*, char*);

// remove a key-value pair from the table
// if remove success, return 1, otherwise 0
int symtab_remove(symtab*, char*);


int symtab_size(symtab*);

void symtab_print(symtab*);

#endif  /* SYMTAB_H */
