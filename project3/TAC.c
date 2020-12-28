#include "TAC.h"

char* new_value(){
    v_num++;
    char *v = malloc(sizeof(char));
    sprintf(v, "v%d", v_num);
    return v;
}

char* new_temp(){
    t_num++;
    char *t = malloc(sizeof(char));
    sprintf(t, "t%d", t_num);
    return t;
}

char* new_label(){
    lb_num++;
    char *lb = malloc(sizeof(char));
    sprintf(lb, "label%d", lb_num);
    return lb;
}




//////////////////IRID_ID_pair/////////////////
IRID_ID_pair* init_ID_pair(char *IRID, char *ID){
    IRID_ID_pair *pair = (IRID_ID_pair*) malloc(sizeof(IRID_ID_pair));
    pair->ID = ID;
    pair->IRID = IRID;
    return pair;
}





//////////Lists//////////////////////
SymList* SymList_init(){
    SymList *head = (SymList*) malloc(sizeof(SymList));
    head->data = NULL;
    head->next = NULL;
    return head;
}

void SymList_insert(SymList *list, IRID_ID_pair *data){
    SymList *ptr = (SymList*) malloc(sizeof(SymList));
    ptr->data = data;
    ptr->next = list->next;
    list->next = ptr;
}

char* SymList_lookup(SymList *list, char *ID){
    SymList *ptr = list->next;
    while (ptr != NULL)
    {
        IRID_ID_pair *pair = (IRID_ID_pair*) ptr->data;
        if (strcmp(pair->ID, ID) == 0)
        {
            return pair->IRID;
        }
        ptr = ptr->next;
    }
    return NULL;
}

myList* myList_init(){
    myList *head = (myList*) malloc(sizeof(myList));
    head->data = NULL;
    head->next = NULL;
    return head;
}

void myList_insert(myList *list, char *data){
    myList *ptr = (myList*) malloc(sizeof(myList));
    ptr->data = data;
    ptr->next = list->next;
    list->next = ptr;
}





///////////////////IRInstruction//////////////////////////
IRInstruction* create_IRInstruction(char* target, int op, char* arg1, char* arg2){
    IRInstruction *IR = (IRInstruction*) malloc(sizeof(IRInstruction));
    IR->index = instruction_index;
    IR->target = target;
    IR->op = op;
    IR->arg1 = arg1;
    IR->arg2 = arg2;
    IR->next = NULL;
    IR->previous = NULL;
    instruction_index++;
    build_IRInstruction_list(IR);
    return IR;
}

IRInstruction* init_IRInstruction(){
    IRInstruction *IR = (IRInstruction*) malloc(sizeof(IRInstruction));
    IR->index = instruction_index;
    IR->target = NULL;
    IR->op = -1;
    IR->arg1 = NULL;
    IR->arg2 = NULL;
    IR->next = NULL;
    IR->previous = NULL;
    instruction_index++;
    End = IR;
    return IR;
}

void build_IRInstruction_list(IRInstruction *ir){
    End->next = ir;
    ir->previous = End;
    End = ir;
}

void print_IRInstrucion(FILE *fp){
    IRInstruction *ptr = Start->next;
    while (ptr != NULL)
    {
        // fprintf(fp, "%d ", ptr->index);
        if (ptr->op == _LABEL)
        {
            fprintf(fp, "LABEL %s :\n", ptr->target);
        }else if (ptr->op == _FUNCTION)
        {
            fprintf(fp, "FUNCTION %s :\n", ptr->target);
        }else if (ptr->op == _ASSIGN)
        {
            fprintf(fp, "%s := %s\n", ptr->target, ptr->arg1);
        }else if (ptr->op == _PLUS)
        {
            fprintf(fp, "%s := %s + %s\n", ptr->target, ptr->arg1, ptr->arg2);
        }else if (ptr->op == _MINUS)
        {
            fprintf(fp, "%s := %s - %s\n", ptr->target, ptr->arg1, ptr->arg2);
        }else if (ptr->op == _MUL)
        {
            fprintf(fp, "%s := %s * %s\n", ptr->target, ptr->arg1, ptr->arg2);
        }else if (ptr->op == _DIV)
        {
            fprintf(fp, "%s := %s / %s\n", ptr->target, ptr->arg1, ptr->arg2);
        }else if (ptr->op == _GOTO)
        {
            fprintf(fp, "GOTO %s\n", ptr->target);
        }else if (ptr->op == _EQ)
        {
            fprintf(fp, "IF %s == %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _NE)
        {
            fprintf(fp, "IF %s != %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _LT)
        {
            fprintf(fp, "IF %s < %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _LE)
        {
            fprintf(fp, "IF %s <= %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _GT)
        {
            fprintf(fp, "IF %s > %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _GE)
        {
            fprintf(fp, "IF %s >= %s GOTO %s\n", ptr->arg1, ptr->arg2, ptr->target);
        }else if (ptr->op == _RETURN)
        {
            fprintf(fp, "RETURN %s\n", ptr->arg1);
        }else if (ptr->op == _PARAM)
        {
            fprintf(fp, "PARAM %s\n", ptr->target);
        }else if (ptr->op == _ARG)
        {
            fprintf(fp, "ARG %s\n", ptr->arg1);
        }else if (ptr->op == _CALL)
        {
            fprintf(fp, "%s := CALL %s\n", ptr->target, ptr->arg1);
        }else if (ptr->op == _READ)
        {
            fprintf(fp, "READ %s\n", ptr->target);
        }else if (ptr->op == _WRITE)
        {
            fprintf(fp, "WRITE %s\n", ptr->arg1);
        }else if (ptr->op == _EMPTY)
        {
            fprintf(fp, "\n");
        }
        
        ptr = ptr->next;
    }
    
}





////////////////translate methods///////////////////////////
void translate_Program(Node *node, FILE *fp){
    printf("translate_Program\n");
    t_num = 0;
    v_num = 0;
    lb_num = 0;
    instruction_index = 0;
    symList = SymList_init();
    // printf("after init symList\n");
    Start = init_IRInstruction();
    // printf("after init IRInstruction\n");
    translate_ExtDefList(node->children[1]);
    optimization();
    print_IRInstrucion(fp);

    // printf("***********print symList****************\n");
    // SymList * ptr = symList->next;
    // while (ptr != NULL)
    // {
    //     printf("%s : %s\n", ptr->data->IRID, ptr->data->ID);
    //     ptr = ptr->next;
    // }
}

void translate_ExtDefList(Node *node){
    printf("translate_ExtDefList\n");
    if (node->childrenNum > 0)
    {
        translate_ExtDef(node->children[0]);
        create_IRInstruction(NULL, _EMPTY, NULL, NULL);
        translate_ExtDefList(node->children[1]);
    }
}

void translate_ExtDef(Node *node){
    printf("translate_ExtDef\n");
    //仅考虑 Specifier FunDec CompSt 的情况
    if (node->childrenNum == 3 && strcmp(node->children[1]->name, "FunDec") == 0)
    {
        translate_FunDec(node->children[1]);
        translate_CompSt(node->children[2]);
    }
}

void translate_FunDec(Node *node){
    printf("translate_FunDec\n");
    create_IRInstruction(node->children[0]->text, _FUNCTION, NULL, NULL);
    if (node->childrenNum == 4) //有参数的情况
    {
        translate_VarList(node->children[2]);
    }
}

void translate_VarList(Node *node){
    printf("translate_VarList\n");
    translate_ParamDec(node->children[0]);
    if (node->childrenNum > 1)
    {
        translate_VarList(node->children[2]);
    }
}

void translate_ParamDec(Node *node){
    printf("translate_ParamDec\n");
    Node *vardec = node->children[1];
    char *v = new_value();
    SymList_insert(symList, init_ID_pair(v, vardec->children[0]->text));
    IRInstruction *IR = create_IRInstruction(v, _PARAM, NULL, NULL);
}

void translate_CompSt(Node *node){
    printf("translate_CompSt\n");
    translate_DefList(node->children[1]);
    translate_StmtList(node->children[2]);
}

void translate_DefList(Node *node){
    printf("translate_DefList\n");
    if (node->childrenNum > 0)
    {
        translate_Def(node->children[0]);
        translate_DefList(node->children[1]);
    }
}

void translate_Def(Node *node){
    printf("translate_Def\n");
    translate_DecList(node->children[1]);
}

void translate_DecList(Node *node){
    printf("translate_DecList\n");
    translate_Dec(node->children[0]);
    if (node->childrenNum > 1)
    {
        translate_DecList(node->children[2]);
    }
}

void translate_Dec(Node *node){
    printf("translate_Dec\n");
    Node *vardec = node->children[0];
    char *v = new_value();
    SymList_insert(symList, init_ID_pair(v, vardec->children[0]->text));
    if (node->childrenNum == 3) 
    {
        Node *exp = node->children[2];
        char *t = new_temp();
        translate_Exp(exp, t);
        create_IRInstruction(v, _ASSIGN, t, NULL);
    }
}

void translate_Exp(Node *node, char *t){
    printf("translate_Exp\n");
    //INT
    if (node->childrenNum == 1 && strcmp(node->children[0]->name, "INT") == 0)
    {
        char *immNum = malloc(sizeof(char));
        sprintf(immNum, "#%s", node->children[0]->text);
        create_IRInstruction(t, _ASSIGN, immNum, NULL);
        // create_IRInstruction(t, _ASSIGN, "#0", NULL);
    }
    //ID
    else if (node->childrenNum == 1 && strcmp(node->children[0]->name, "ID") == 0)
    {
        char *v = SymList_lookup(symList, node->children[0]->text);
        create_IRInstruction(t, _ASSIGN, v, NULL);
    }
    //Exp ASSIGN Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "ASSIGN") == 0)
    {
        char *v = SymList_lookup(symList, node->children[0]->children[0]->text);
        if (strcmp(node->children[2]->children[0]->name, "READ") == 0)
        {
            translate_Exp(node->children[2], v);
        }else
        {
            char *t1 = new_temp();
            translate_Exp(node->children[2], t1);
            create_IRInstruction(v, _ASSIGN, t1, NULL);
            // create_IRInstruction(t, _ASSIGN, v, NULL);
        }
    }
    //Exp PLUS Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "PLUS") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(t, _PLUS, t1, t2);
    }
    //Exp MINUS Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "MINUS") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(t, _MINUS, t1, t2);
    }
    //Exp MUL Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "MUL") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(t, _MUL, t1, t2);
    }
    //Exp DIV Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "DIV") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(t, _DIV, t1, t2);
    }
    //MINUS Exp
    else if (node->childrenNum == 2 && strcmp(node->children[0]->name, "MINUS") == 0)
    {
        char *t1 = new_temp();
        translate_Exp(node->children[1], t1);
        create_IRInstruction(t, _MINUS, "#0", t1);
    }
    //EXP cond EXP, NOT EXP
    else if ((node->childrenNum == 3 && strcmp(node->children[0]->name, "Exp") == 0 && strcmp(node->children[2]->name, "Exp") == 0)
    || node->childrenNum == 2 && strcmp(node->children[0]->name, "NOT") == 0)
    {
        char *lb1 = new_label();
        char *lb2 = new_label();
        create_IRInstruction(t, _ASSIGN, "#0", NULL);
        translate_cond_Exp(node, lb1, lb2);
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        create_IRInstruction(t, _ASSIGN, "#1", NULL);
        create_IRInstruction(lb2, _LABEL, NULL, NULL);
    }
    //READ
    else if (strcmp(node->children[0]->name, "READ") == 0)
    {
        create_IRInstruction(t, _READ, NULL, NULL);
    }
    //WRITE
    else if (strcmp(node->children[0]->name, "WRITE") == 0)
    {
        char *t1 = new_temp();
        translate_Exp(node->children[2], t1);
        create_IRInstruction(NULL, _WRITE, t1, NULL);
    }
    //ID LP RP
    else if (node->childrenNum == 3 && strcmp(node->children[0]->name, "ID") == 0)
    {
        create_IRInstruction(t, _CALL, node->children[0]->text, NULL);
    }
    //ID LP Args RP
    else if (node->childrenNum == 4 && strcmp(node->children[0]->name, "ID") == 0)
    {
        myList *argList = myList_init();
        translate_Args(node->children[2], argList);
        myList *ptr = argList->next;
        while (ptr != NULL)
        {
            create_IRInstruction(NULL, _ARG, (char*)ptr->data, NULL);
            ptr = ptr->next;
        }
        create_IRInstruction(t, _CALL, node->children[0]->text, NULL);
        free(argList);
    }
    //LP Exp RP
    else if (node->childrenNum == 3 && strcmp(node->children[0]->name, "LP") == 0)
    {
        translate_Exp(node->children[1], t);
    }
        
}

void translate_Args(Node *node, myList *argList){
    printf("translate_Args\n");
    char *v = new_value();
    translate_Exp(node->children[0], v);
    myList_insert(argList, v);
    if (node->childrenNum > 1)
    {
        translate_Args(node->children[2], argList);
    }
}

void translate_cond_Exp(Node *node, char *lb_t, char *lb_f){
    printf("translate_cond_Exp\n");
    //Exp EQ Exp
    if (node->childrenNum == 3 && strcmp(node->children[1]->name, "EQ") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _EQ, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp NE Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "NE") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _NE, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp LT Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "LT") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _LT, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp LE Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "LE") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _LE, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp GT Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "GT") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _GT, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp GE Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "GE") == 0)
    {
        char *t1 = new_temp();
        char *t2 = new_temp();
        translate_Exp(node->children[0], t1);
        translate_Exp(node->children[2], t2);
        create_IRInstruction(lb_t, _GE, t1, t2);
        create_IRInstruction(lb_f, _GOTO, NULL, NULL);
    }
    //Exp AND Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "AND") == 0)
    {
        char *lb1 = new_label();
        translate_cond_Exp(node->children[0], lb1, lb_f);
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        translate_cond_Exp(node->children[2], lb_t, lb_f);
    }
    //Exp OR Exp
    else if (node->childrenNum == 3 && strcmp(node->children[1]->name, "OR") == 0)
    {
        char *lb1 = new_label();
        translate_cond_Exp(node->children[0], lb_t, lb1);
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        translate_cond_Exp(node->children[2], lb_t, lb_f);
    }
    //NOT Exp
    else if (node->childrenNum == 2 && strcmp(node->children[0]->name, "NOT") == 0)
    {
        translate_cond_Exp(node->children[1], lb_f, lb_t);
    }
}

void translate_StmtList(Node *node){
    printf("translate_StmtList\n");
    if (node->childrenNum > 0)
    {
        translate_Stmt(node->children[0]);
        translate_StmtList(node->children[1]);
    }
}

void translate_Stmt(Node *node){
    printf("translate_Stmt\n");
    //Exp
    // printf("children 0 name : %s\n", node->children[0]->name);
    if (strcmp(node->children[0]->name, "Exp") == 0)
    {
        char *t = new_temp();
        // printf("before translate_Exp, t is %s\n", t);
        translate_Exp(node->children[0], t);
    }
    //CompSt
    else if (strcmp(node->children[0]->name, "CompSt") == 0)
    {
        translate_CompSt(node->children[0]);
    }
    //RETURN Exp
    else if (strcmp(node->children[0]->name, "RETURN") == 0)
    {
        char *t = new_temp();
        translate_Exp(node->children[1], t);
        create_IRInstruction(NULL, _RETURN, t, NULL);
    }
    //IF LP Exp RP Stmt
    else if (strcmp(node->children[0]->name, "IF") == 0 && node->childrenNum == 5)
    {
        char *lb1 = new_label();
        char *lb2 = new_label();
        translate_cond_Exp(node->children[2], lb1, lb2);
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        translate_Stmt(node->children[4]);
        create_IRInstruction(lb2, _LABEL, NULL, NULL);
    }
    //IF LP Exp RP Stmt ELSE Stmt
    else if (strcmp(node->children[0]->name, "IF") == 0 && node->childrenNum == 7)
    {
        char *lb1 = new_label();
        char *lb2 = new_label();
        char *lb3 = new_label();
        translate_cond_Exp(node->children[2], lb1, lb2);
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        translate_Stmt(node->children[4]);
        create_IRInstruction(lb3, _GOTO, NULL, NULL);
        create_IRInstruction(lb2, _LABEL, NULL, NULL);
        translate_Stmt(node->children[6]);
        create_IRInstruction(lb3, _LABEL, NULL, NULL);
    }
    //WHILE LP Exp RP Stmt
    else if (strcmp(node->children[0]->name, "WHILE") == 0)
    {
        char *lb1 = new_label();
        char *lb2 = new_label();
        char *lb3 = new_label();
        create_IRInstruction(lb1, _LABEL, NULL, NULL);
        translate_cond_Exp(node->children[2], lb2, lb3);
        create_IRInstruction(lb2, _LABEL, NULL, NULL);
        translate_Stmt(node->children[4]);
        create_IRInstruction(lb1, _GOTO, NULL, NULL);
        create_IRInstruction(lb3, _LABEL, NULL, NULL);
    }
    
}






/////////////////optimize methods///////////////////////////
void optimization(){
    //分割成basic block，并对block做局部优化
    IRInstruction *block_begin_ptr = Start->next;
    while (block_begin_ptr->next != NULL)
    {
        IRInstruction *block_end_ptr = block_begin_ptr->next;
        while (1)
        {
            if (block_end_ptr->next == NULL)//没有更多Instructions
            {
                break;
            }else if (block_end_ptr->op == _LABEL)//下一个Instruction是一个label，即下一个指令是新的leader instruction
            {
                break;
            }else if (block_end_ptr->op == _LT || block_end_ptr->op == _LE || block_end_ptr->op == _GT
                    || block_end_ptr->op == _GE || block_end_ptr->op ==_EQ || block_end_ptr->op == _NE)//if...goto...情况
            {
                block_end_ptr = block_end_ptr->next;
                break;
            }
            block_end_ptr = block_end_ptr->next;                
        }
        //block内部优化
        optimize_block(block_begin_ptr, block_end_ptr);
        block_begin_ptr = block_end_ptr;
    }
    //全局优化
    optimize_global();
}

//指针左闭右开
void optimize_block(IRInstruction *begin, IRInstruction *end){
    // printf("current block start at %d, end at %d\n", begin->index, end->index-1);
    
    IRInstruction *ptr = begin;
    while (ptr->index < end->index)
    {
        //解决常数计算
        if ((ptr->op == _PLUS || ptr->op == _MINUS || ptr->op == _MUL || ptr->op == _DIV)
            && ptr->arg1[0] == '#' && ptr->arg2[0] == '#')
        {
            caculate(ptr);
            ptr->op = _ASSIGN;
            ptr->arg2 = NULL;
        }else if (ptr->op == _MINUS && strcmp(ptr->arg1, ptr->arg2) == 0)
        {
            ptr->op = _ASSIGN;
            ptr->arg1 = "#0";
            ptr->arg2 = NULL;
        }else if (ptr->op == _MUL && (strcmp(ptr->arg1, "#1") == 0 || strcmp(ptr->arg2, "#1") == 0))
        {
            ptr->op = _ASSIGN;
            ptr->arg1 = strcmp(ptr->arg1, "#1") == 0 ? ptr->arg2 : ptr->arg1;
            ptr->arg2 = NULL;
        }else if (ptr->op == _DIV && strcmp(ptr->arg2, "#1") == 0)
        {
            ptr->op = _ASSIGN;
            ptr->arg2 = NULL;
        }

        //解决复写传播
        if (ptr->op == _ASSIGN && (ptr->target[0] == 't' 
            || (ptr->target[0] == 'v' && ptr->arg1[0] != 't')))//如果是temp = value的情况不做替换
        {
            char *early_def = ptr->arg1;
            char *curr_var = ptr->target;
            IRInstruction *ptr1 = ptr->next;
            while (1)
            {
                //复写传播的范围(从赋值开始到target被赋值或arg1被赋值或block结束)
                if((ptr1->target != NULL && (strcmp(ptr1->target, curr_var) == 0 || strcmp(ptr1->target, early_def) == 0)) 
                    || ptr1->index >= end->index){
                    break;
                }else if (ptr1->arg1 != NULL && strcmp(ptr1->arg1, curr_var) == 0)
                {
                    ptr1->arg1 = early_def;
                }else if (ptr1->arg2 != NULL && strcmp(ptr1->arg2, curr_var) == 0)
                {
                    ptr1->arg2 = early_def;
                }
                ptr1 = ptr1->next;
            }
        }
        ptr = ptr->next;
    }

    //解决公共子表达式子
}

void optimize_global(){
    //从上往下遍历instructions，删除无用赋值
    int changed = 0;
    do{
        IRInstruction *uptodown_ptr = Start->next;
        changed = 0;
        while (uptodown_ptr != NULL)
        {
            //有 := 的代码才有可能是无用赋值
            if (uptodown_ptr->op == _ASSIGN || uptodown_ptr->op == _PLUS || uptodown_ptr->op == _MINUS
                || uptodown_ptr->op == _MUL || uptodown_ptr->op == _DIV)
            {
                int used = var_is_use(uptodown_ptr);
                if (!used)
                {
                    delete_instruction(uptodown_ptr);
                    changed = 1;
                }
            }
            uptodown_ptr = uptodown_ptr->next;
        }
    } while (changed);
    // printf("已删除无用赋值\n");
    //从下往上遍历instruction做优化
    IRInstruction *bottomup_ptr = End;
    while (bottomup_ptr->index > Start->index)
    {
        //删除无用label
        if (bottomup_ptr->op == _LABEL)
        {
            bottomup_ptr = delete_useless_label(bottomup_ptr);
        }
        //简化i=i+1这种赋值的代码
        else if (bottomup_ptr->op == _ASSIGN && bottomup_ptr->arg1[0] == 't')
        {
            bottomup_ptr = simplify_ii1(bottomup_ptr);
        }
        else{
            bottomup_ptr = bottomup_ptr->previous;
        }
    }
}

IRInstruction* simplify_ii1(IRInstruction *ptr){
    IRInstruction *ptr1 = ptr->previous;
    while (1)
    {
        if (ptr1->target != NULL && strcmp(ptr1->target, ptr->arg1) == 0)
        {
            ptr1->target = ptr->target;
            ptr = ptr->previous;
            delete_instruction(ptr->next);
            break;
        }
        ptr1 = ptr1->previous;
    }
    return ptr;
}

IRInstruction* delete_useless_label(IRInstruction *ptr){
    //删除内容为空的label
    if (ptr->next->op == _EMPTY)
    {
        IRInstruction *ptr1 = ptr->previous;
        while (1)
        {
            if (ptr1->op == _GOTO || 
                ptr1->op == _LT || ptr1->op == _LE || ptr1->op == _GT
                || ptr1->op == _GE || ptr1->op ==_EQ || ptr1->op == _NE)
            {
                if (strcmp(ptr1->target, ptr->target) == 0)
                {
                    delete_instruction(ptr1);
                    ptr = ptr->previous;
                    delete_instruction(ptr->next);
                    break;
                }
            }
            ptr1 = ptr1->previous;
        }
    }
    //删除堆积在一起的冗余的label
    else if (ptr->previous->op == _LABEL)
    {
        IRInstruction *tobe_delete = ptr->previous;
        IRInstruction *ptr1 = tobe_delete->previous;
        while (1)
        {
            if (ptr1->op == _GOTO || 
                ptr1->op == _LT || ptr1->op == _LE || ptr1->op == _GT
                || ptr1->op == _GE || ptr1->op ==_EQ || ptr1->op == _NE)
            {
                if (strcmp(ptr1->target, tobe_delete->target) == 0)
                {
                    ptr1->target = ptr->target;
                    break;
                }
            }
            ptr1 = ptr1->previous;
        }
        delete_instruction(tobe_delete);
    }
    else
    {
        ptr = ptr->previous;
    }
    return ptr;
}

int var_is_use(IRInstruction *ptr){
    IRInstruction *ptr1 = ptr->next;
    int used = 0;
    while (1)
    {
        //指令尾或下一次被赋值 就停止搜索
        if (ptr1 == NULL || (ptr1->target != NULL && strcmp(ptr1->target, ptr->target) == 0))
        {
            break;
        }
        if (ptr1->arg1 != NULL && strcmp(ptr1->arg1, ptr->target) == 0
            || ptr1->arg2 != NULL && strcmp(ptr1->arg2, ptr->target) == 0)
        {
            used = 1;
        }
        else if (ptr1->op == _GOTO)//处理while回路
        {
            char *label = ptr1->target;
            IRInstruction *ptr2 = Start->next;
            while (ptr2 != NULL && ptr2->index < ptr->index)
            {
                if (ptr2->op == _LABEL && strcmp(ptr2->target, label) == 0)
                {
                    while (ptr2 != NULL && ptr2->index < ptr->index)
                    {
                        if (ptr2->arg1 != NULL && strcmp(ptr2->arg1, ptr->target) == 0
                            || ptr2->arg2 != NULL && strcmp(ptr2->arg2, ptr->target) == 0)
                        {
                            used = 1;
                        }
                        ptr2 = ptr2->next;
                    }
                    break;
                }
                ptr2 = ptr2->next;
            }
        }
        ptr1 = ptr1->next;
    }
    return used;
}

void delete_instruction(IRInstruction *ir){
    ir->previous->next = ir->next;
    ir->next->previous = ir->previous;
    ir->next = NULL;
    ir->previous = NULL;
    free(ir);
}

void caculate(IRInstruction *ptr){
    int a = get_num(ptr->arg1);
    int b = get_num(ptr->arg2);
    int result = 0;
    if (ptr->op == _PLUS)
    {
        result = a + b;
    }else if (ptr->op == _MINUS)
    {
        result = a - b;
    }else if (ptr->op == _MUL)
    {
        result = a * b;
    }else if (ptr->op == _DIV)
    {
        result = a / b;
    }
    char *immNum = malloc(sizeof(char));
    sprintf(immNum, "#%d", result);
    ptr->arg1 = immNum;
}

int get_num(char *s){
    int length = strlen(s) -1;
    char* num_s = (char*)malloc(sizeof(char)*length);
    for (int i = 0; i < length; i++)
    {
        num_s[i] = s[i+1];
    }
    return atoi(num_s);
}