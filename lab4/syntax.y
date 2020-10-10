%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}

%token LC RC LB RB COLON COMMA
%token STRING NUMBER
%token TRUE FALSE VNULL
%%

Json:
      Value
    | Value COMMA error { puts("comma after the close, recovered"); }
    | Value RB error { puts("extra close, recovered"); }
    | Value STRING error { puts("misplaced quoted value, recovered"); }
    ;
Value:
      Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    | NUMBER NUMBER error { puts("numbers cannot have leading zeros, recovered"); }
    ;
Object:
      LC RC
    | LC Members RC
    ;
Members:
      Member
    | Member COMMA Members
    | Member COMMA error { puts("extra comma, recovered"); }
    ;
Member:
      STRING COLON Value
    | STRING Value error { puts("missing colon, recovered"); }
    | STRING COLON COLON error { puts("double colon, recovered"); }
    | STRING COMMA Value error { puts("comma instead of colon, recovered"); }
    ;
Array:
      LB RB
    | LB Values RB
    | LB Values RC error { puts("unmatched right bracket, recovered"); }
    | LB Values error { puts("unclosed array, recovered"); }
    ;
Values:
      Value
    | Value COMMA Values
    | Value COMMA error { puts("extra comma, recovered"); }
    | Value COMMA COMMA error { puts("double extra comma, recovered"); }
    | COMMA Value error { puts("missing value, recovered"); }
    | Value COLON Values error { puts("colon instead of comma, recovered"); }
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}
