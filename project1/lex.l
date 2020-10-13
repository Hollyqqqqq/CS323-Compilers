%{
    #include"syntax.tab.h"
    #include"tree.c"
%}
%option noyywrap

type int|float|char
INT 0|[1-9][0-9]*|0[xX]0|0[xX][1-9a-fA-F][0-9a-fA-F]*
FLOAT [0-9]+\.[0-9]+
ascii [\x00-\xff]
CHAR \'{ascii}\'|\'\\x0\'|\'\\x[1-9a-fA-F][0-9a-fA-F]*\'
ID [_a-zA-Z][0-9_a-zA-Z]*
empty [ \r\t]

%%
"struct" { 
    yylval.node = createTerminal("STRUCT", "", yylineno, 0);
    return STRUCT; 
    }
"if" { 
    yylval.node = createTerminal("IF", "", yylineno, 0);
    return IF; 
    }
"else" {
    yylval.node = createTerminal("ELSE", "", yylineno, 0);
    return ELSE; 
    }
"while" { 
    yylval.node = createTerminal("WHILE", "", yylineno, 0);
    return WHILE; 
    }
"return" { 
    yylval.node = createTerminal("RETURN", "", yylineno, 0);
    return RETURN; 
    }
{type} {
    printf("TYPE\n");
    yylval.node = createTerminal("TYPE", yytext, yylineno, 1);
    return TYPE; 
    }
{INT} { 
    printf("INT\n");
    yylval.node = createTerminal("INT", yytext, yylineno, 1);
    return INT; 
    }
{FLOAT} {
    printf("FLOAT\n");
    yylval.node = createTerminal("FLOAT", yytext, yylineno, 1);
    return FLOAT; 
    }
{CHAR} {
    printf("CHAR\n");
    yylval.node = createTerminal("CHAR", yytext, yylineno, 1);
    return CHAR; 
    }
{ID} {
    printf("ID\n");
    yylval.node = createTerminal("ID", yytext, yylineno, 1);
    return ID; 
    }
"." { 
    yylval.node = createTerminal("DOT", "", yylineno, 0);
    return DOT; 
    }
";" { 
    yylval.node = createTerminal("SEMI", "", yylineno, 0);
    return SEMI; 
    }
"," { 
    yylval.node = createTerminal("COMMA", "", yylineno, 0);
    return COMMA; 
    }
"=" { 
    yylval.node = createTerminal("ASSIGN", "", yylineno, 0);
    return ASSIGN; 
    }
"<" { 
    yylval.node = createTerminal("LT", "", yylineno, 0);
    return LT; 
    }
"<=" { 
    yylval.node = createTerminal("LE", "", yylineno, 0);
    return LE; 
    }
">" { 
    yylval.node = createTerminal("GT", "", yylineno, 0);
    return GT; 
    }
">=" { 
    yylval.node = createTerminal("GE", "", yylineno, 0);
    return GE; 
    }
"!=" { 
    yylval.node = createTerminal("NE", "", yylineno, 0);
    return NE; 
    }
"==" { 
    yylval.node = createTerminal("EQ", "", yylineno, 0);
    return EQ; 
    }
"+" { 
    yylval.node = createTerminal("PLUS", "", yylineno, 0);
    return PLUS; 
    }
"-" { 
    yylval.node = createTerminal("MINUS", "", yylineno, 0);
    return MINUS; 
    }
"*" { 
    yylval.node = createTerminal("MUL", "", yylineno, 0);
    return MUL; 
    }
"/" { 
    yylval.node = createTerminal("DIV", "", yylineno, 0);
    return DIV; 
    }
"&&" { 
    yylval.node = createTerminal("AND", "", yylineno, 0);
    return AND; 
    }
"||" { 
    yylval.node = createTerminal("OR", "", yylineno, 0);
    return OR; 
    }
"!" { 
    yylval.node = createTerminal("NOT", "", yylineno, 0);
    return NOT; 
    }
"(" { 
    yylval.node = createTerminal("LP", "", yylineno, 0);
    return LP; 
    }
")" { 
    yylval.node = createTerminal("RP", "", yylineno, 0);
    return RP; 
    }
"[" { 
    yylval.node = createTerminal("LB", "", yylineno, 0);
    return LB; 
    }
"]" { 
    yylval.node = createTerminal("RB", "", yylineno, 0);
    return RB; 
    }
"{" { 
    yylval.node = createTerminal("LC", "", yylineno, 0);
    return LC; 
    }
"}" { 
    yylval.node = createTerminal("RC", "", yylineno, 0);
    return RC; 
    }

\n { yylineno++; }
{empty} {}
. { printf("error type A at Line %d:  Mysterious lexeme %s\n", yylineno, yytext); }
