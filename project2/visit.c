#include "visit.h"
//symtab_insert要检查是否成功
void visit_Program(Node *node, FILE* fp1){
    fp = fp1;
    printf("visit_Program\n");
    scope_stack = stack_init();
    push(scope_stack, symtab_init());
    visit_ExtDefList(node->children[1]);
    printf("visit_Program done!\n");
    printf("table size is %d\n", symtab_size(peek(scope_stack)));
    printf("******************* print stack ********************\n");
    stack_print(scope_stack);
}

void visit_ExtDefList(Node *node){
    printf("visit_ExtDefList\n");
    if (node->childrenNum > 0)
    {
        visit_ExtDef(node->children[0]);
        visit_ExtDefList(node->children[1]);
    }
}

void visit_ExtDef(Node *node){
    printf("visit_ExtDef\n");
    TypeTuple *type = visit_Specifier(node->children[0]);//要返回一个元组
    if (node->childrenNum == 3)
    {
        if (node->children[1]->children[0]->isTerminal)//Specifier FunDec CompSt
        {
            IDType *func = visit_FunDec(node->children[1], type);
            visit_CompSt(node->children[2], func);
        }else // Specifier ExtDecList SEMI
        {
            visit_ExtDecList(node->children[1], type);
        }
    }
    //skip "ExtDef: Specifier SEMI"
}

TypeTuple* visit_Specifier(Node *node){
    printf("visit_Specifier\n");
    Node *child = node->children[0];
    if (child->isTerminal)
    {
        // printf("in visit_Specifier, type is %s\n", child->text);
        if (strcmp(child->text, "int") == 0) return create_TypeTuple(INT_, "");
        else if (strcmp(child->text, "float") == 0) return create_TypeTuple(FLOAT_, "");
        else return create_TypeTuple(CHAR_, ""); //char
    }else
    {
        return visit_StructSpecifier(child);
    }    
}

TypeTuple* visit_StructSpecifier(Node *node){
    printf("visit_StructSpecifier\n");
    if (node->childrenNum == 5)
    {
        IDType *id = create_IDType(STRUCTURE);
        id->name = node->children[1]->text;
        int is_success = symtab_insert(peek(scope_stack), id->name, id);
        if (!is_success) error_type15(node->children[1]->lineNum, node->children[1]->text);
        printf("table size is %d\n", symtab_size(peek(scope_stack)));           
        // push(scope_stack, symtab_init());//再建一个table，结构体里的scope
        id->structure = visit_DefList(node->children[3]);//返回一个FieldList？
        printf("%s id.structure is null? %d\n", id->name, id->structure == NULL);
        FieldList *temp = id->structure;
        while (temp != NULL)
        {
            // printf("current struct member is %s, cat is %d\n", temp->name, temp->type->category);
            temp = temp->next;
        }
        
        // pop(scope_stack);//当结构体建立完成就pop最顶部的table
    }
    return create_TypeTuple(STRUCTURE, node->children[1]->text);
}

FieldList* visit_DefList(Node *node){
    printf("visit_DefList\n");
    if (node->childrenNum == 2)
    {
        // printf("enter child num = 2\n");
        FieldList *list = visit_Def(node->children[0]);
        FieldList *temp = list;
        while (temp->next != NULL)
        {
            // printf("current struct member is %s, cat is %d\n", temp->name, temp->type->category);
            temp = temp->next;
        }
        // printf("in visit_DefList, list is null? %d\n", list==NULL);
        temp->next = visit_DefList(node->children[1]);
        // FieldList *temp = list;
        // while (temp != NULL)
        // {
        //     printf("current struct member is %s, cat is %d\n", temp->name, temp->type->category);
        //     temp = temp->next;
        // }
        return list;
    }else
    {
        return NULL;
    }
}

FieldList* visit_Def(Node *node){
    printf("visit_Def\n");
    TypeTuple *type = visit_Specifier(node->children[0]);
    FieldList *list = visit_DecList(node->children[1], type);
    // FieldList *temp = list;
    // while (temp != NULL)
    // {
    //     printf("current struct member is %s, cat is %d\n", temp->name, temp->type->category);
    //     temp = temp->next;
    // }
    return list;
}

FieldList* visit_DecList(Node *node, TypeTuple *type){
    printf("visit_DecList\n");
    FieldList *list = create_FieldList();
    // printf("in visit_DecList, type is %d\n", type->type);
    TypeTuple *cpType = copy_TypeTuple(type);//list 都要注意copy！！
    visit_Dec(node->children[0], type, list);//返回一个IDType
    if (node->childrenNum != 1)
    {
        // printf("DecList current is %s, cat is %d\n", list->name, list->type->category);
        list->next = visit_DecList(node->children[2], cpType);
        // printf("DecList after add next, next is %s, cat is %d\n", list->next->name, list->next->type->category);
    }   
    return list;
}

void visit_Dec(Node *node, TypeTuple *type, FieldList *list){
    printf("visit_Dec\n");
    IDType *id = visit_VarDec(node->children[0], type, list);
    if(node->childrenNum == 3){//判断等号两边的合法性
        IDType *id1 = visit_Exp(node->children[2]);
        int is_same = idtype_cmp(id, id1);
        if (id->category != UNKNOW_ && id1->category != UNKNOW_ && !is_same) error_type5(node->children[1]->lineNum);
    }
}

IDType* visit_VarDec(Node *node, TypeTuple *type, FieldList *list){
    printf("visit_VarDec\n");
    IDType *id;
    if (node->childrenNum == 1)
    {
        id = create_IDType(type->type);
        id->name = type->name;
        if (type->type == STRUCTURE)
        {
            id->structure = symtab_lookup(peek(scope_stack), type->name)->structure;
        }
        
        for (int i = 0; i < type->dim; i++)
        {
            IDType *temp = id;
            id = create_IDType(ARRAY);
            id->name = "";
            id->array = create_Array();
            id->array->base = temp;
            id->array->size = type->dims[i];
        }
        // printf("insert name is %s, cat is %d\n", node->children[0]->text, id->category);
        int is_success = symtab_insert(peek(scope_stack), node->children[0]->text, id);
        if (!is_success) error_type3(node->children[0]->lineNum, node->children[0]->text);
        printf("table size is %d\n", symtab_size(peek(scope_stack)));
        if (list != NULL)
        {
            // printf("list is not null\n");
            list->name = node->children[0]->text;
            list->type = id;
        }else
        {
            // printf("list is null\n");
        }
        return id;
    }else//array
    {
        type->dims[type->dim] = atoi(node->children[2]->text);
        type->dim += 1;
        return visit_VarDec(node->children[0], type, list);
    }
}

void visit_ExtDecList(Node* node, TypeTuple *type){
    printf("visit_ExtDecList\n");
    visit_VarDec(node->children[0], type, NULL);
    if (node->childrenNum != 1)
    {
        visit_ExtDecList(node->children[2], type);
    }
}

IDType* visit_FunDec(Node *node, TypeTuple *type){
    printf("visit_FunDec\n");
    IDType *id = create_IDType(FUNCTION);
    id->name = node->children[0]->text;
    id->function = create_Function();
    id->function->returnType = create_IDType(type->type);
    id->function->returnType->name = type->name;
    int is_success = symtab_insert(peek(scope_stack), id->name, id);
    if(!is_success) error_type4(node->children[0]->lineNum, node->children[0]->text);
    printf("table size is %d\n", symtab_size(peek(scope_stack)));
    if (node->childrenNum == 4)
    {
        id->function->params = visit_VarList(node->children[2]);
    }else
    {
        id->function->params = NULL;
    }
    return id;
}

FieldList* visit_VarList(Node *node){
    printf("visit_VarList\n");
    FieldList *list = create_FieldList();
    list = visit_ParamDec(node->children[0]);
    if (node->childrenNum == 3)
    {
        list->next = visit_VarList(node->children[2]);
    }
    return list;
}

FieldList* visit_ParamDec(Node *node){
    printf("visit_ParamDec\n");
    TypeTuple *type = visit_Specifier(node->children[0]);
    FieldList *list = create_FieldList();
    visit_VarDec(node->children[1], type, list);
    return list;
}

void visit_CompSt(Node *node, IDType* func){
    printf("visit_CompSt\n");
    visit_DefList(node->children[1]);
    visit_StmtList(node->children[2], func);
}

void visit_StmtList(Node *node, IDType* func){
    printf("visit_StmtList\n");
    if (node->childrenNum != 0)
    {
        visit_Stmt(node->children[0], func);
        visit_StmtList(node->children[1], func);
    }
}

void visit_Stmt(Node *node, IDType* func){
    printf("visit_Stmt\n");
    if (node->children[0]->isTerminal)//retrun, if, while
    {
        if (strcmp(node->children[0]->name, "RETURN") == 0)//return
        {
            // printf("enter return\n");
            IDType *id = visit_Exp(node->children[1]);
            // printf("func return type is %d\n", func == NULL);
            int is_same = idtype_cmp(func->function->returnType, id);
            if(id->category != UNKNOW_ && !is_same) error_type8(node->children[0]->lineNum);
        }else//if, while
        {
            // printf("enter if or while\n");
            visit_Exp(node->children[2]);
            visit_Stmt(node->children[4], func);
            if (node->childrenNum == 7)//带有else的情况
            {
                visit_Stmt(node->children[6], func);
            }
        }       
    }else if (node->childrenNum == 1)//CompSt
    {
        visit_CompSt(node->children[0], func);
    }else//Exp SEMI
    {
        visit_Exp(node->children[0]);
    }
}

IDType* visit_Exp(Node *node){
    printf("visit_Exp\n");
    if (strcmp(node->children[0]->name, "ID") == 0)//ID开头的产生式
    {
        printf("enter exp start with ID\n");
        Node* IDnode = node->children[0];
        IDType *id = symtab_lookup(peek(scope_stack), IDnode->text);
        if (node->childrenNum == 1)//ID
        {
            // printf("enter ID \n");
            if (id == NULL) error_type1(IDnode->lineNum, IDnode->text);
            // printf("ready to exit ID \n");
        }else //ID LP (Args) RP
        {
            // printf("enter ID LP (Args) RP\n");
            if (id == NULL) error_type2(IDnode->lineNum, IDnode->text);
            else if (id->category != FUNCTION)
            {
                error_type11(IDnode->lineNum);
            }
            if (node->childrenNum == 4)
            {
                FieldList *argList = visit_Args(node->children[2]); //就算func没有被定义，传入的参数也要被检查
                // printf("visit_Args done\n");
                if (id != NULL && id->category == FUNCTION) {
                    // printf("enter cmp func args\n");
                    int is_same = cmp_func_args(id->function->params, argList);
                    if (!is_same) error_type9(node->children[0]->lineNum);
                }
            }
        }
        if (id == NULL)
        {
            printf("could not find id %s\n", IDnode->text);
            id = create_IDType(UNKNOW_);
        }
        // printf("ready to return id, %d\n", id->category);
        return id;
    }else if (node->childrenNum == 1)//int float char情况
    {
        printf("enter int or float or char\n");
        if (strcmp(node->children[0]->name, "INT") == 0) {return create_IDType(INT_);}
        else if (strcmp(node->children[0]->name, "FLOAT") == 0) return create_IDType(FLOAT_);
        else return create_IDType(CHAR_);
    }else if(node->childrenNum == 3 && strcmp(node->children[1]->name, "ASSIGN")==0)
    {//Exp ASSIGN Exp
        printf("enter exp assign exp\n");
        //先判断left value是否合法
        if (strcmp(node->children[0]->children[0]->name, "ID")==0){
            // printf("************************enter ID\n");
            if (node->children[0]->childrenNum != 1){
                // printf("************************rvalue1\n");
                error_type6(node->children[0]->children[0]->lineNum);
            }else
            {
                IDType *temp = symtab_lookup(peek(scope_stack), node->children[0]->children[0]->text);
                if (temp != NULL && temp->category == FUNCTION)
                {
                    // printf("************************rvalue2\n");
                    error_type6(node->children[0]->children[0]->lineNum);
                }
            }
        }else if (node->children[0]->childrenNum == 3 && strcmp(node->children[0]->children[1]->name, "DOT")!=0)
        {
            // printf("************************rvalue3\n");
            error_type6(node->children[0]->children[1]->lineNum);
        }else if (node->children[0]->childrenNum <= 2){
            // printf("************************rvalue4\n");
            error_type6(node->children[0]->children[0]->lineNum);
        }
        //再判断具体的Exp
        IDType *id1 = visit_Exp(node->children[0]);
        // printf("left of = is done\n");
        IDType *id2 = visit_Exp(node->children[2]);
        // printf("********************id1 cat is %d, id2 cat is %d\n", id1->category, id2->category);
        int is_same = idtype_cmp(id1, id2);
        if (is_same) return id1;
        else 
        {
            if(id1->category != UNKNOW_ && id2->category != UNKNOW_) {
                error_type5(node->children[1]->lineNum);
                return create_IDType(UNKNOW_);
            }else if (id1->category != UNKNOW_)
            {
                return id1;
            }else
            {
                return id2;
            }
        }
    }else if (node->childrenNum == 3 && 
                strcmp(node->children[0]->name, "Exp")==0 && 
                strcmp(node->children[2]->name, "Exp")==0)
    {//Exp op Exp
        printf("enter exp op exp\n");
        IDType *id1 = visit_Exp(node->children[0]);
        IDType *id2 = visit_Exp(node->children[2]);
        // printf("before idtype_cmp\n");
        int is_same = idtype_cmp(id1, id2);
        // printf("after idtype_cmp\n");
        if (is_same) return id1;
        else 
        {
            // printf("enter else\n");
            if (id1->category != UNKNOW_ && id2->category != UNKNOW_)
            {
                // printf("*************case1\n");
                error_type7(node->children[1]->lineNum);
                // printf("print error done\n");
                return create_IDType(UNKNOW_);
            }else if (id1->category != UNKNOW_){
                // printf("*************case2\n");
                return id1;
            }else
            {
                // printf("*************case3\n");
                return id2;
            }
        }
    }else if (node->childrenNum == 4)//Exp LB EXP RB
    {
        printf("enter exp LB exp RB\n");
        IDType *id = visit_Exp(node->children[0]);
        if (id->category != ARRAY)
        {
            if (id->category != UNKNOW_)
            {
                error_type10(node->children[0]->lineNum);
            }
            return id;
        }
        IDType* id1 = visit_Exp(node->children[2]);
        if (id1->category != INT_)
        {
            error_type12(node->children[2]->lineNum);
        }
        // printf("id array is null ? %d\n", id->array == NULL);
        return id->array->base;
    }else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "DOT") == 0) //Exp DOT ID
    {
        printf("enter exp dot id\n");
        IDType *id = visit_Exp(node->children[0]);
        if (id->category != STRUCTURE)
        {
            // printf("cat is not struct\n");
            error_type13(node->children[1]->lineNum);
        }else
        {
            // printf("cat is struct\n");
            IDType *member = find_struct_member(id, node->children[2]->text);
            // printf("**************is find ? %d\n", member != NULL);
            if (member == NULL) error_type14(node->children[2]->lineNum);
            else
            {
                return member;
            }
        }
        return create_IDType(UNKNOW_);
        
    }else if (node->childrenNum == 3)//LP Exp RP
    {
        return visit_Exp(node->children[1]);
    }else //MINUS Exp, NOT Exp
    {
        return visit_Exp(node->children[1]);
    }
}

FieldList* visit_Args(Node *node){
    printf("visit_Args\n");
    FieldList *list = create_FieldList();
    list->type = visit_Exp(node->children[0]);
    // printf("visit args half done, node.childrennum is %d\n", node->childrenNum);
    if (node->childrenNum == 3)
    {
        // printf("enter children num 3\n");
        list->next = visit_Args(node->children[2]);
    }
    return list;
}

////////////////////////////////////////////////////////////////////////////////////

void error_type1(int line, char *text){
    fprintf(fp, "Error type 1 at line %d: %s variable is used without definition\n", line, text);
}

void error_type2(int line, char* text){
    fprintf(fp, "Error type 2 at line %d: %s function is invoked without definition\n", line, text);
}

void error_type3(int line, char* text){
    fprintf(fp, "Error type 3 at line %d: %s variable is redefined in the same scope\n", line, text);
}

void error_type4(int line, char* text){
    fprintf(fp, "Error type 4 at line %d: %s function is redefined\n", line, text);
}

void error_type5(int line){
    fprintf(fp, "Error type 5 at line %d: unmatching types on both sides of assignment operator\n", line);
}

void error_type6(int line){
    fprintf(fp, "Error type 6 at line %d: rvalue on the left side of assignment operator\n", line);
}

void error_type7(int line){
    fprintf(fp, "Error type 7 at line %d: unmatching operands\n", line);
}

void error_type8(int line){
    fprintf(fp, "Error type 8 at line %d: the function’s return value type mismatches the declared type\n", line);
}

void error_type9(int line){
    fprintf(fp, "Error type 9 at line %d: the function’s arguments mismatch the declared parameters\n", line);
}

void error_type10(int line){
    fprintf(fp, "Error type 10 at line %d: applying indexing operator ([...]) on non-array type variables\n", line);
}

void error_type11(int line){
    fprintf(fp, "Error type 11 at line %d: applying function invocation operator on non-function names\n", line);
}

void error_type12(int line){
    fprintf(fp, "Error type 12 at line %d: array indexing with non-integer type expression\n", line);
}

void error_type13(int line){
    fprintf(fp, "Error type 13 at line %d: accessing member of non-structure variable\n", line);
}

void error_type14(int line){
    fprintf(fp, "Error type 14 at line %d: accessing an undefined structure member\n", line);
}

void error_type15(int line, char *text){
    fprintf(fp, "Error type 15 at line %d: redefine structure type %s\n", line, text);
}