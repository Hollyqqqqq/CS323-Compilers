typedef struct Json
{
    char name[32];
    enum {OBJECT, ARRAY, STRING, NUMBER, BOOLEAN, VNULL} category;
    union{
        struct Object *object;
        struct Array *array;
        char *string;
        enum {INT, FLOAT} number;
        enum {TRUE, FALSE} boolean;
        enum {NULL} vnull;
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