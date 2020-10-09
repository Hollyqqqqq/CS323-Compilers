%{
    #include"lex.yy.c"
    int result;
    void yyerror(const char *s){ result = 0;}
%}
%token LP RP LB RB LC RC
%%
String: TrueMatch {result = 1;};
TrueMatch:
    | TrueMatch Pair
    ;
Pair: LB TrueMatch RB
    | LC TrueMatch RC
    | LP TrueMatch RP
    ;
%%

int validParentheses(char *expr){
    yy_scan_string(expr);
    yyparse();
    return result;
}


