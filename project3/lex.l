%{
    #include"syntax.tab.h"
    #include "node.h"
    #include "visit.h"
    #include "idtype.h"
    #include "stack.h"
    #include "symtab.h"
    #include "TAC.h"
    int errors = 0;
    FILE *fp;
%}
%option noyywrap

type int|float|char
decimal 0|[1-9][0-9]*
heximal 0[xX](0|[1-9a-fA-F][0-9a-fA-F]*)
WrongHex 0[xX]([0-9a-fA-F]*[e-zE-Z][0-9a-fA-F]*|0[0-9a-fA-F]*)
INT {decimal}|{heximal}
FLOAT [0-9]+\.[0-9]+
CHAR \'.\'|\'\\x0\'|\'\\x[1-9a-fA-F][0-9a-fA-F]?\'
WrongCHAR \'\\x[0-9a-zA-Z]*\'|\'..+\'
ID [_a-zA-Z][0-9_a-zA-Z]*
WrongID [0-9]+{ID}
commentSingle \/\/.*\n
commentMulMiddle .*\n
commentMul \/\*{commentMulMiddle}*.*\*\/
INCLUDE #include\ (<.*\..*>|\".*\..*\")
empty [ \r\t]
UnknownOP [|&]
UnknownStr [0-9a-zA-Z]*['"@$#?`~]([0-9a-zA-Z]*[@$#?`~]*)*
%%
{commentSingle} {
    // printf("commentSingle\n");
    yylineno++;
}
{commentMul} {
    char *ptr = yytext;
    int i = 0;
    while (i < yyleng){
        if(ptr[i] == '\n'){
            yylineno++;
        }
        i++;
    }
    // printf("commentMul\n");
}
{INCLUDE} {
    // printf("INCLUDE\n");
    yylval.node = createTerminal("INCLUDE", "", yylineno, 0);
    return INCLUDE; 
}
"struct" { 
    // printf("STRUCT\n");
    yylval.node = createTerminal("STRUCT", "", yylineno, 0);
    return STRUCT; 
    }
"if" { 
    // printf("IF\n");
    yylval.node = createTerminal("IF", "", yylineno, 0);
    return IF; 
    }
"else" {
    // printf("ELSE\n");
    yylval.node = createTerminal("ELSE", "", yylineno, 0);
    return ELSE; 
    }
"while" {
    // printf("WHILE\n"); 
    yylval.node = createTerminal("WHILE", "", yylineno, 0);
    return WHILE; 
    }
"for" {
    // printf("FOR\n"); 
    yylval.node = createTerminal("FOR", "", yylineno, 0);
    return FOR; 
    }
"return" { 
    // printf("RETURN\n");
    yylval.node = createTerminal("RETURN", "", yylineno, 0);
    return RETURN; 
    }
"read" {
    yylval.node = createTerminal("READ", "", yylineno, 0);
    return READ; 
}
"write" {
    yylval.node = createTerminal("WRITE", "", yylineno, 0);
    return WRITE; 
}
{type} {
    // printf("TYPE\n");
    yylval.node = createTerminal("TYPE", yytext, yylineno, 1);
    return TYPE; 
    }
{INT} { 
    // printf("INT\n");
    yylval.node = createTerminal("INT", yytext, yylineno, 1);
    return INT; 
    }
{WrongHex} {
    // printf("WrongHex\n");
    yylval.node = createTerminal("INT", yytext, yylineno, 1);
    fprintf(fp, "Error type A at Line %d: wrong hex int %s\n", yylineno, yytext);
    errors++;
    return INT; 
}
{FLOAT} {
    // printf("FLOAT\n");
    yylval.node = createTerminal("FLOAT", yytext, yylineno, 1);
    return FLOAT; 
    }
{CHAR} {
    // printf("CHAR\n");
    yylval.node = createTerminal("CHAR", yytext, yylineno, 1);
    return CHAR; 
    }
{WrongCHAR} {
    // printf ("WrongCHAR\n");
    yylval.node = createTerminal("CHAR", yytext, yylineno, 1);
    fprintf(fp, "Error type A at Line %d: wrong char %s\n", yylineno, yytext);
    errors++;
    return CHAR;
}
{ID} {
    // printf("ID\n");
    yylval.node = createTerminal("ID", yytext, yylineno, 1);
    return ID; 
    }
{WrongID} {
    // printf("WrongID\n");
    fprintf(fp, "Error type A at Line %d: Wrong id %s\n", yylineno, yytext);
    yylval.node = createTerminal("ID", yytext, yylineno, 1);
    errors++;
    return ID; 
}
"." { 
    // printf("DOT\n");
    yylval.node = createTerminal("DOT", "", yylineno, 0);
    return DOT; 
    }
";" { 
    // printf("SEMI\n");
    yylval.node = createTerminal("SEMI", "", yylineno, 0);
    return SEMI; 
    }
"；" { 
    // printf("ChineseSEMI\n");
    yylval.node = createTerminal("SEMI", "", yylineno, 0);
    fprintf(fp, "Error type A at Line %d: Chinese SEMI\n", yylineno);
    errors++;
    return SEMI; 
    }
"," { 
    // printf("COMMA\n");
    yylval.node = createTerminal("COMMA", "", yylineno, 0);
    return COMMA; 
    }
"，" { 
    // printf("ChineseCOMMA\n");
    yylval.node = createTerminal("COMMA", "", yylineno, 0);
    fprintf(fp, "Error type A at Line %d: Chinese COMMA\n", yylineno);
    errors++;
    return COMMA; 
    }
"++" {
    // printf("INC\n");
    yylval.node = createTerminal("INC", "", yylineno, 0);
    return INC;
}
"--" {
    // printf("DEC\n");
    yylval.node = createTerminal("DEC", "", yylineno, 0);
    return DEC;
}
"+=" {
    // printf("ADDASIGN\n");
    yylval.node = createTerminal("ADDASIGN", "", yylineno, 0);
    return ADDASSIGN;
}
"-=" {
    // printf("SUBASIGN\n");
    yylval.node = createTerminal("SUBASIGN", "", yylineno, 0);
    return SUBASSIGN;
}
"*=" {
    // printf("MULASIGN\n");
    yylval.node = createTerminal("MULASIGN", "", yylineno, 0);
    return MULASSIGN;
}
"/=" {
    // printf("DIVASIGN\n");
    yylval.node = createTerminal("DIVASIGN", "", yylineno, 0);
    return DIVASSIGN;
}
"%=" {
    // printf("MODASIGN\n");
    yylval.node = createTerminal("MODASIGN", "", yylineno, 0);
    return MODASSIGN;
}
"=" { 
    // printf("ASSIGN\n");
    yylval.node = createTerminal("ASSIGN", "", yylineno, 0);
    return ASSIGN; 
    }
"<" { 
    // printf("LT\n");
    yylval.node = createTerminal("LT", "", yylineno, 0);
    return LT; 
    }
"<=" { 
    // printf("LE\n");
    yylval.node = createTerminal("LE", "", yylineno, 0);
    return LE; 
    }
">" { 
    // printf("GT\n");
    yylval.node = createTerminal("GT", "", yylineno, 0);
    return GT; 
    }
">=" { 
    // printf("GE\n");
    yylval.node = createTerminal("GE", "", yylineno, 0);
    return GE; 
    }
"!=" { 
    // printf("NE\n");
    yylval.node = createTerminal("NE", "", yylineno, 0);
    return NE; 
    }
"==" { 
    // printf("EQ\n");
    yylval.node = createTerminal("EQ", "", yylineno, 0);
    return EQ; 
    }
"+" { 
    // printf("PLUS\n");
    yylval.node = createTerminal("PLUS", "", yylineno, 0);
    return PLUS; 
    }
"-" { 
    // printf("MINUS\n");
    yylval.node = createTerminal("MINUS", "", yylineno, 0);
    return MINUS; 
    }
"*" { 
    // printf("MUL\n");
    yylval.node = createTerminal("MUL", "", yylineno, 0);
    return MUL; 
    }
"/" { 
    // printf("DIV\n");
    yylval.node = createTerminal("DIV", "", yylineno, 0);
    return DIV; 
    }
"%" {
    // printf("MOD\n");
    yylval.node = createTerminal("MOD", "", yylineno, 0);
    return MOD; 
}
"&&" { 
    // printf("AND\n");
    yylval.node = createTerminal("AND", "", yylineno, 0);
    return AND; 
    }
"||" { 
    // printf("OR\n");
    yylval.node = createTerminal("OR", "", yylineno, 0);
    return OR; 
    }
{UnknownOP} {
    // printf("UnknownOP\n");
    yylval.node = createTerminal("UNKNOWNOP", "", yylineno, 0);
    fprintf(fp, "Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
    errors++;
    return UNKNOWNOP; 
}
"!" { 
    // printf("NOT\n");
    yylval.node = createTerminal("NOT", "", yylineno, 0);
    return NOT; 
    }
"(" { 
    // printf("LP\n");
    yylval.node = createTerminal("LP", "", yylineno, 0);
    return LP; 
    }
")" { 
    // printf("RP\n");
    yylval.node = createTerminal("RP", "", yylineno, 0);
    return RP; 
    }
"[" { 
    // printf("LB\n");
    yylval.node = createTerminal("LB", "", yylineno, 0);
    return LB; 
    }
"]" { 
    // printf("RB\n");
    yylval.node = createTerminal("RB", "", yylineno, 0);
    return RB; 
    }
"{" { 
    // printf("LC\n");
    yylval.node = createTerminal("LC", "", yylineno, 0);
    return LC; 
    }
"}" { 
    // printf("RC\n");
    yylval.node = createTerminal("RC", "", yylineno, 0);
    return RC; 
    }

\n { yylineno++; }
{empty} {}
{UnknownStr} {
    // printf("UNKNOWN\n");
    yylval.node = createTerminal("UNKNOWN", "", yylineno, 0);
    fprintf(fp, "Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
    errors++;
    return UNKNOWN;
}
. { 
    // printf("UNKNOWN\n");
    yylval.node = createTerminal("UNKNOWN", "", yylineno, 0);
    fprintf(fp, "Error type A at Line %d: unknown lexeme %s\n", yylineno, yytext);
    errors++;
    return UNKNOWN;
    }
