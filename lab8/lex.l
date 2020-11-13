%{
    #include"syntax.tab.h"
    #include"stack.h"
    Stack *stack;
%}
%option noyywrap

unic u[0-9a-fA-F]{4}
esc \\([\"\\/bfnrt]|{unic})
scp [^"\\\x00-\x1f]
string \"({esc}|{scp})*\"

int 0|[1-9][0-9]*
frac \.[0-9]+
exp [Ee][+-]?[0-9]+
number -?{int}{frac}?{exp}?

empty [ \n\r\t]

%%

"{" { push(stack); return LC; }
"}" { return RC; }
"[" { return LB; }
"]" { return RB; }
":" { return COLON; }
"," { return COMMA; }

"true" { return TRUE; }
"false" { return FALSE; }
"null" { return VNULL; }

{string} { 
    yylval.string = (char*)malloc(sizeof(char)*strlen(yytext));
    strcpy(yylval.string,yytext);
    return STRING;
}
{number} { return NUMBER; }

{empty} {}
