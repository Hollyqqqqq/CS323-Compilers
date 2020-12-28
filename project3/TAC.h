#ifndef TAC_H
#define TAC_H

#include "node.h"



int t_num;
int v_num;
int lb_num;
char* new_temp();
char* new_value();
char* new_label();




//////////////////IRID_ID_pair/////////////////
typedef struct IRID_ID_pair
{
    char *IRID;
    char *ID;
} IRID_ID_pair;
IRID_ID_pair* init_ID_pair(char *IRID, char *ID);






//////////Lists//////////////////////
typedef struct SymList{
    IRID_ID_pair *data;
    struct SymList *next;
} SymList;
SymList *symList;
SymList* SymList_init();
void SymList_insert(SymList *list, IRID_ID_pair *data);
char* SymList_lookup(SymList *list, char *ID);

typedef struct myList{
    char *data;
    struct myList *next;
} myList;
myList* myList_init();
void myList_insert(myList *list, char *data);





/////////////////IRInstruction/////////////////////////////
typedef struct IRInstruction
{
    int index;
    char* target;
    enum {_LABEL, _FUNCTION, _ASSIGN, _PLUS, _MINUS, _MUL, _DIV, _ADDR, _VAL, _GOTO, _LT, _LE, _GT, _GE, _EQ, _NE, _RETURN, _DEC, _PARAM, _ARG, _CALL, _READ, _WRITE, _EMPTY} op;
    char* arg1;
    char* arg2;
    struct IRInstruction *next;
    struct IRInstruction *previous;
} IRInstruction;
int instruction_index;
IRInstruction *Start;//中间代码的head
IRInstruction *End;//Instruction list构建过程中的最后一行代码
IRInstruction* create_IRInstruction(char* target, int op, char* arg1, char* arg2);
IRInstruction* init_IRInstruction();
void build_IRInstruction_list(IRInstruction *ir);
void print_IRInstrucion(FILE *fp);





////////////////translate methods///////////////////////////
void translate_Program(Node *node, FILE *fp);
void translate_ExtDefList(Node *node);
void translate_ExtDef(Node *node);
void translate_FunDec(Node *node);
void translate_VarList(Node *node);
void translate_ParamDec(Node *node);
void translate_CompSt(Node *node);
void translate_DefList(Node *node);
void translate_Def(Node *node);
void translate_DecList(Node *node);
void translate_Dec(Node *node);
void translate_Exp(Node *node, char *t);
void translate_Args(Node *node, myList *argList);
void translate_cond_Exp(Node *node, char *lb_t, char *lb_f);
void translate_StmtList(Node *node);
void translate_Stmt(Node *node);





//////////////optimize methods////////////////////////////////////////
void optimization();
void optimize_block(IRInstruction *begin, IRInstruction *end);
void optimize_global();
IRInstruction* simplify_ii1(IRInstruction *ptr);
IRInstruction* delete_useless_label(IRInstruction *ptr);
int var_is_use(IRInstruction *ptr);
void delete_instruction(IRInstruction *ir);
void caculate(IRInstruction *ptr);
int get_num(char *s);

#endif