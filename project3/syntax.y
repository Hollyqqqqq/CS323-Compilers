%{
    #include"lex.yy.c"
    void yyerror(const char*);
    struct Node* root;
    void yyerror(const char *s);
    void yySEMIerror();
    void yyRPerror();
    void yyRCerror();
    void yyMissSpec();
%}
%union{
    struct Node* node;
}

%token <node> INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE FOR RETURN INCLUDE READ WRITE
%token <node> DOT SEMI COMMA
%token <node> ASSIGN LT LE GT GE NE EQ
%token <node> PLUS MINUS MUL DIV MOD INC DEC ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN
%token <node> AND OR NOT
%token <node> LP RP LB RB LC RC
%token <node> UNKNOWN UNKNOWNOP

%type <node> Program ExtDefList ExtDef ExtDecList IncludeList
%type <node> Specifier StructSpecifier
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args

%nonassoc LOWER_ELSE
%nonassoc ELSE
%left UNKNOWNOP
%right ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE 
%left PLUS MINUS
%left MUL DIV MOD
%right NOT INC DEC
%left LP RP LB RB DOT


%%


Program: 
      IncludeList ExtDefList {root=createNonTerminal("Program"); buildTree(root, 2, $1, $2);}
    ;
IncludeList:
      INCLUDE IncludeList { $$=createNonTerminal("IncludeList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("IncludeList"); buildTree($$, 0); }
    ;
ExtDefList: 
      ExtDef ExtDefList { $$=createNonTerminal("ExtDefList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("ExtDefList"); buildTree($$, 0); }
    ;
ExtDef:
      Specifier ExtDecList SEMI { $$=createNonTerminal("ExtDef"); buildTree($$, 3, $1, $2, $3); }
    | Specifier ExtDecList error { yySEMIerror(); }
    | Specifier SEMI { $$=createNonTerminal("ExtDef"); buildTree($$, 2, $1, $2); }
    | Specifier error { yySEMIerror(); }
    | Specifier FunDec CompSt { $$=createNonTerminal("ExtDef"); buildTree($$, 3, $1, $2, $3); }
    | error CompSt {errors++; fprintf(fp, "Error type B at Line %d: Missing Specifier and FunDec\n", yylineno);}
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
    | ID LP VarList error { yyRPerror(); }
    | ID LP RP { $$=createNonTerminal("FunDec"); buildTree($$, 3, $1, $2, $3); }
    | ID LP error { yyRPerror(); }
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
    | Exp error { yySEMIerror(); }
    | CompSt { $$=createNonTerminal("Stmt"); buildTree($$, 1, $1); }
    | RETURN Exp SEMI { $$=createNonTerminal("Stmt"); buildTree($$, 3, $1, $2, $3); }
    | RETURN Exp error { yySEMIerror(); }
    | IF LP Exp RP Stmt %prec LOWER_ELSE { $$=createNonTerminal("Stmt"); buildTree($$, 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$=createNonTerminal("Stmt"); buildTree($$, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF LP Exp error { yyRPerror(); }
    | WHILE LP Exp RP Stmt { $$=createNonTerminal("Stmt"); buildTree($$, 5, $1, $2, $3, $4, $5); }
    | WHILE LP Exp error { yyRPerror(); }
    | FOR LP Def Exp SEMI Exp RP Stmt { $$=createNonTerminal("Stmt"); buildTree($$, 8, $1, $2, $3, $4, $5, $6, $7, $8); }
    ;


DefList:
      Def DefList { $$=createNonTerminal("DefList"); buildTree($$, 2, $1, $2); }
    | { $$=createNonTerminal("DefList"); buildTree($$, 0); }
    | error Stmt { yyMissSpec(); }
    ;
Def:
      Specifier DecList SEMI { $$=createNonTerminal("Def"); buildTree($$, 3, $1, $2, $3); }
    | Specifier DecList error { yySEMIerror(); }
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
    | Exp MOD Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp ADDASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp SUBASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp MULASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp DIVASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp MODASSIGN Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | Exp UNKNOWNOP Exp { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | LP Exp RP { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | LP Exp error { yyRPerror(); }
    | MINUS Exp { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | NOT Exp { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | ID LP Args RP { $$=createNonTerminal("Exp"); buildTree($$, 4, $1, $2, $3, $4); }
    | ID LP Args error { yyRPerror(); }
    | ID LP RP { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | ID LP error { yyRPerror(); }
    | READ LP RP { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3);}
    | WRITE LP Exp RP { $$=createNonTerminal("Exp"); buildTree($$, 4, $1, $2, $3, $4); }
    | Exp LB Exp RB { $$=createNonTerminal("Exp"); buildTree($$, 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$=createNonTerminal("Exp"); buildTree($$, 3, $1, $2, $3); }
    | ID INC { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | ID DEC { $$=createNonTerminal("Exp"); buildTree($$, 2, $1, $2); }
    | ID { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | INT { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | FLOAT { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | CHAR { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    | UNKNOWN { $$=createNonTerminal("Exp"); buildTree($$, 1, $1); }
    ;
Args:
      Exp COMMA Args { $$=createNonTerminal("Args"); buildTree($$, 3, $1, $2, $3); }
    | Exp { $$=createNonTerminal("Args"); buildTree($$, 1, $1); }
    ;
%%

void yyerror(const char *s){printf("syntax error\n");}

void yySEMIerror(){
    fprintf(fp, "Error type B at Line %d: Missing semicolon ';'\n", yylineno);
    errors++;
}

void yyRPerror(){
    fprintf(fp, "Error type B at Line %d: Missing closing parenthesis ')'\n", yylineno);
    errors++;
}

void yyRCerror(){
    fprintf(fp, "Error type B at Line %d: Missing closing brace '}'\n", yylineno);
    errors++;
}

void yyMissSpec(){
    fprintf(fp, "Error type B at Line %d: Missing specifier\n", yylineno);
    errors++;
}

int main(int argc, char ** argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    int length = strlen(argv[1])+1;
    char* outPath = (char*)malloc(sizeof(char)*length);
    strcpy(outPath,argv[1]);
    outPath[length-1] = '\0';
    outPath[length-2] = 't';
    outPath[length-3] = 'u';
    outPath[length-4] = 'o';
    fp = fopen(outPath, "w+");
    length -= 1;
    char* outPath1 = (char*)malloc(sizeof(char)*length);
    strcpy(outPath1,argv[1]);
    outPath1[length-1] = '\0';
    outPath1[length-2] = 'r';
    outPath1[length-3] = 'i';
    FILE *fp1 = fopen(outPath1, "w+");
    //词法、语法分析
    yyparse();
    if (errors == 0){
        printTree(root, 0, fp);
        //语义分析
        // visit_Program(root, fp);
        fclose(fp);
        //生成中间代码
        translate_Program(root, fp1);
        fclose(fp1);
    }
    return 0;
}