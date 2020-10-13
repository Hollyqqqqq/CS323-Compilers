%{
    #include"lex.yy.c"
    void yyerror(const char*);
    struct Node* root;
%}
%union{
    struct Node* node;
}

%token <node> INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE RETURN 
%token <node> DOT SEMI COMMA
%token <node> ASSIGN LT LE GT GE NE EQ
%token <node> PLUS MINUS MUL DIV
%token <node> AND OR NOT
%token <node> LP RP LB RB LC RC

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args

%nonassoc LOWER_ELSE
%nonassoc ELSE
%right ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE 
%left PLUS MINUS
%left MUL DIV
%right NOT
%left LP RP LB RB DOT


%%


Program: 
      ExtDefList {root=createNonTerminal("Program"); buildTree(root, 1, $1);}
    ;
ExtDefList: 
      ExtDef ExtDefList { $$=createNonTerminal("ExtDefList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("ExtDefList"); buildTree($$, 0); }
    ;
ExtDef:
      Specifier ExtDecList SEMI { $$=createNonTerminal("ExtDef"); buildTree($$, 3, $1, $2, $3); }
    | Specifier SEMI { $$=createNonTerminal("ExtDef"); buildTree($$, 2, $1, $2); }
    | Specifier FunDec CompSt { $$=createNonTerminal("ExtDef"); buildTree($$, 3, $1, $2, $3); }
    ;
ExtDecList:
      VarDec { $$=createNonTerminal("ExtDecList"); buildTree($$, 1, $1); }
    | VarDec COMMA ExtDecList { $$=createNonTerminal("ExtDecList"); buildTree($$, 3, $1, $2, $3); }
    ;


Specifier:
      TYPE { $$=createNonTerminal("Specifier"); buildTree($$, 1, $1); }
    | StructSpecifier { $$=createNonTerminal("Specifier"); buildTree($$, 1, $1); }
    ;
StructSpecifier:
      STRUCT ID LC DefList RC { $$=createNonTerminal("StructSpecifier"); buildTree($$, 5, $1, $2, $3, $4, $5); }
    | STRUCT ID { $$=createNonTerminal("StructSpecifier"); buildTree($$, 2, $1, $2); }
    ;


VarDec:
      ID { $$=createNonTerminal("VarDec"); buildTree($$, 1, $1); }
    | VarDec LB INT RB { $$=createNonTerminal("VarDec"); buildTree($$, 4, $1, $2, $3, $4); }
    ;
FunDec:
      ID LP VarList RP { $$=createNonTerminal("FunDec"); buildTree($$, 4, $1, $2, $3, $4); }
    | ID LP RP { $$=createNonTerminal("FunDec"); buildTree($$, 3, $1, $2, $3); }
    ;
VarList:
      ParamDec COMMA VarList { $$=createNonTerminal("VarList"); buildTree($$, 3, $1, $2, $3); }
    | ParamDec { $$=createNonTerminal("VarList"); buildTree($$, 1, $1); }
    ;
ParamDec:
      Specifier VarDec { $$=createNonTerminal("ParamDec"); buildTree($$, 2, $1, $2); }
    ;



CompSt:
      LC DefList StmtList RC { $$=createNonTerminal("CompSt"); buildTree($$, 4, $1, $2, $3, $4); }
      ;
StmtList:
      Stmt StmtList { $$=createNonTerminal("StmtList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("StmtList"); buildTree($$, 0); }
    ;
Stmt:
      Exp SEMI { $$=createNonTerminal("Stmt"); buildTree($$, 2, $1, $2); }
    | CompSt { $$=createNonTerminal("Stmt"); buildTree($$, 1, $1); }
    | RETURN Exp SEMI { $$=createNonTerminal("Stmt"); buildTree($$, 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_ELSE{ $$=createNonTerminal("Stmt"); buildTree($$, 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$=createNonTerminal("Stmt"); buildTree($$, 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$=createNonTerminal("Stmt"); buildTree($$, 5, $1, $2, $3, $4, $5); }
    ;


DefList:
      Def DefList { $$=createNonTerminal("DefList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("DefList"); buildTree($$, 0); }
    ;
Def:
      Specifier DecList SEMI { $$=createNonTerminal("Def"); buildTree($$, 3, $1, $2, $3); }
    ;
DecList:
      Dec { $$=createNonTerminal("DecList"); buildTree($$, 1, $1); }
    | Dec COMMA DecList { $$=createNonTerminal("DecList"); buildTree($$, 3, $1, $2, $3); }
    ;
Dec:
      VarDec { $$=createNonTerminal("Dec"); buildTree($$, 1, $1); }
    | VarDec ASSIGN Exp { $$=createNonTerminal("Dec"); buildTree($$, 3, $1, $2, $3); }
    ;


Exp:
      Exp ASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp AND Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp OR Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp LT Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp LE Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp GT Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp GE Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp NE Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp EQ Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp PLUS Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp MINUS Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp MUL Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp DIV Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | LP Exp RP { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | MINUS Exp { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | NOT Exp { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | ID LP Args RP { $$=createNonTerminal("Exp"); buildTree($$, 4, $1, $2, $3, $4); }
    | ID LP RP { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp LB Exp RB { $$=createNonTerminal("Exp"); buildTree($$, 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | ID { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | INT { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | FLOAT { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | CHAR { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    ;
Args:
      Exp COMMA Args { $$=createNonTerminal("Args"); buildTree($$, 3, $1, $2, $3); }
    | Exp { $$=createNonTerminal("Args"); buildTree($$, 1, $1); }
    ;
%%

int main(int argc, char ** argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    int length = strlen(argv[1]);
    char* outPath = (char*)malloc(sizeof(char)*length);
    strcpy(outPath,argv[1]);
    outPath[length-1] = 't';
    outPath[length-2] = 'u';
    outPath[length-3] = 'o';
    FILE *fp = fopen(outPath, "w+");
    printTree(root, 0, fp);
    return 0;
}