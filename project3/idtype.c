#include "idtype.h"

IDType* create_IDType(int type){
    IDType* idtype = (IDType*) malloc(sizeof(IDType));
    idtype->category = type;
    idtype->name = "";
    return idtype;
}

Array* create_Array(){
    Array* array = (Array*) malloc(sizeof(Array));
    return array;
}

FieldList* create_FieldList(){
    FieldList* list = (FieldList*) malloc(sizeof(FieldList));
    list->next = NULL;
    return list;
}

Function* create_Function(){
    Function* func = (Function*) malloc(sizeof(Function));
    func->returnType = NULL;
    func->params = NULL;
    return func;
}

int idtype_cmp(IDType *id1, IDType *id2){
    // if (id1 == NULL || id2 == NULL)
    // {
    //     return 0;
    // }
    if (id1->category == FUNCTION)
    {
        id1 = id1->function->returnType;
    }
    if (id2->category == FUNCTION)
    {
        id2 = id2->function->returnType;
    }
    
    if (id1->category == id2->category && strcmp(id1->name, id2->name) == 0)
    {
        return 1; //相同
    }
    
    return 0;
}

IDType* find_struct_member(IDType *id, char *member){
    // printf("id.structure is null? %d\n", id->structure == NULL);
    FieldList *members = id->structure;
    while (members != NULL)
    {
        // printf("\t current member is %s\n", members->name);
        if (strcmp(members->name, member) == 0) return members->type;
        members = members->next;
    }
    return NULL;
}

int cmp_func_args(FieldList *l1, FieldList *l2){
    while (l1 != NULL && l2 != NULL)
    {
        if (l1->type->category != l2->type->category)
        {
            return 0;
        }
        l1 = l1->next;
        l2 = l2->next;
    }
    if (l1 != NULL || l2 != NULL)
    {
        return 0;
    }
    return 1;
}

TypeTuple* create_TypeTuple(int typeNum, char *name){
    TypeTuple* type = (TypeTuple*) malloc(sizeof(TypeTuple));
    type->type = typeNum;
    type->name = name;
    type->dim = 0;
    return type;
}

TypeTuple* copy_TypeTuple(TypeTuple *origin){
    TypeTuple *type = create_TypeTuple(origin->type, origin->name);
    type->dim = origin->dim;
    for (int i = 0; i < type->dim; i++)
    {
        type->dims[i] = origin->dims[i];
    }
    return type;
}


void IDType_print(IDType *id){
    // printf("enter IDType_print\n");
    char category[32];
    switch (id->category)
    {
    case INT_:
        sprintf(category, "int");
        break;
    case FLOAT_:
        sprintf(category, "float");
        break;
    case CHAR_:
        sprintf(category, "char");
        break;
    case ARRAY:
        sprintf(category, "[%d]", id->array->size);
        IDType_print(id->array->base);
        break;
    case STRUCTURE:
        sprintf(category, "struct");
        break;
    case FUNCTION:
        //waiting,还没有验证方法的参数       
        sprintf(category, "%d%s func", id->function->returnType->category, id->function->returnType->name);
        break;
    default:
        sprintf(category, "unknow type");
        break;
    }
    printf("%s %s", category, id->name);
}