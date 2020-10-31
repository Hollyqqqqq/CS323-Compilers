#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Json
{
    enum {OBJECT_, ARRAY_, STRING_, NUMBER_, BOOLEAN_, VNULL_} category;
    union{
        struct Object *object;
        struct Array *array;
        char *string;
        double number;
        int boolean;
    };
} Json;

typedef struct Object
{
    char *string;
    struct Json *value;
    struct Object *members;
} Object;

typedef struct Array
{
    struct Json *value;
    struct Array *values;
} Array;

Json* createJson(){
    Json* json = (Json *) malloc(sizeof(Json));
    return json;
}

Object* createObject(){
    Object* object = (Object*) malloc(sizeof(Object));
    return object;
}

Array* createArray(){
    Array* array = (Array*) malloc(sizeof(Array));
    return array;
}