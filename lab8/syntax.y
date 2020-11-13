%{
    #include <stdbool.h>
    #include"lex.yy.c"
    #include"stack.h"
    void yyerror(const char*);
    int table_insert(int* table, char* key);

    int is_valid = 1;  // use this value
%}
%union{
    char *string;
}

%token LC RC LB RB COLON COMMA
%token <string> STRING
%token NUMBER
%token TRUE FALSE VNULL
%%

Json:
      Value
    ;
Value:
      Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    ;
Object:
      LC RC { pop(stack);}
    | LC Members RC { pop(stack);}
    ;
Members:
      Member
    | Member COMMA Members
    ;
Member:
      STRING COLON Value {
            int result = table_insert(peek(stack), $1);
            if(result == 0) is_valid = 0;
        }
    ;
Array:
      LB RB
    | LB Values RB
    ;
Values:
      Value
    | Value COMMA Values
    ;
%%

int table_insert(int* table, char* key){
    int index = key[1] - 'a';
    if (table[index] == 1)
    {
        printf("duplicate key: %s\n", key);
        return 0;
    }else
    {
        table[index] = 1;
        return 1;
    }
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
    stack = stack_init();
    yyparse();
    if(is_valid) {
        printf("%d\n", is_valid);
    }
    return 0;
}
