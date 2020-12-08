#ifndef IDTYPE_H
#define IDTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct IDType { 
    char *name;//如果是int float char array，name为"", func的name应该是什么？？？
    enum { INT_, FLOAT_, CHAR_, ARRAY, STRUCTURE, FUNCTION, UNKNOW_} category; 
    union {
        struct Array *array;
        struct FieldList *structure; 
        struct Function *function;
    };
} IDType;

typedef struct Array {
    struct IDType *base; 
    int size;
} Array;

typedef struct FieldList { 
    char *name;
    struct IDType *type; 
    struct FieldList *next; 
} FieldList;

typedef struct Function {//有点问题，return怎么检查？？？？
    struct IDType *returnType;
    struct FieldList *params;
} Function;

typedef struct TypeTuple{
    int type;
    char *name;//如果是int float char array，name为""
    int dim;//记录维度，普通变量维度0， 数组变量维度1起
    int dims[10];
}TypeTuple;

IDType* create_IDType(int);
Array* create_Array();
FieldList* create_FieldList();
Function* create_Function();
int idtype_cmp(IDType *id1, IDType *id2);
IDType* find_struct_member(IDType *id, char *member);
int cmp_func_args(FieldList *l1, FieldList *l2);

TypeTuple* create_TypeTuple(int, char*);
TypeTuple* copy_TypeTuple(TypeTuple *origin);


void IDType_print(IDType*);
#endif