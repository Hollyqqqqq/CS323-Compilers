#ifndef VISIT_H
#define VISIT_H

#include "node.h"
#include "idtype.h"
#include "symtab.h"
#include "stack.h"

FILE *fp;
Stack* scope_stack;

void visit_Program(Node*, FILE*);
void visit_ExtDefList(Node*);
void visit_ExtDef(Node*);
TypeTuple* visit_Specifier(Node*);
TypeTuple* visit_StructSpecifier(Node*);
FieldList* visit_DefList(Node*);
FieldList* visit_Def(Node*);
FieldList* visit_DecList(Node*, TypeTuple*);
void visit_Dec(Node *node, TypeTuple *type, FieldList *list);
IDType* visit_VarDec(Node *node, TypeTuple *type, FieldList *list);
void visit_ExtDecList(Node* node, TypeTuple *type);
IDType* visit_FunDec(Node *node, TypeTuple *type);
FieldList* visit_VarList(Node *node);
FieldList* visit_ParamDec(Node *node);
void visit_CompSt(Node*, IDType*);
void visit_StmtList(Node*, IDType*);
void visit_Stmt(Node*, IDType*);
IDType* visit_Exp(Node*);
FieldList* visit_Args(Node *node);


void error_type1(int, char*);
void error_type2(int, char*);
void error_type3(int, char*);
void error_type4(int, char*);
void error_type5(int);
void error_type6(int);
void error_type7(int);
void error_type8(int);
void error_type9(int);
void error_type10(int);
void error_type11(int);
void error_type12(int);
void error_type13(int);
void error_type14(int);
void error_type15(int, char*);

#endif