%{
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>
void yyerror (char *s);
int yylex();
extern int line_num;


%}


//Codes corresponding to different tokens
%union {int num; char id; char arr[25];}         /* Yacc definitions */
%start prog
%token PROGRAM
%token VAR
%token BEGIN_WORD
%token END
%token ENDPERIOD
%token INTEGER_KEYWORD
%token REAL_KEYWORD
%token FOR
%token READ
%token WRITE
%token TO
%token DO
%token SEMICOLON
%token COLON
%token COMMA
%token ASSIGNMENT
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE
%token LEFTPARENTHESIS
%token RIGHTPARENTHESIS
%token <arr> IDENTIFIER
%token INTEGER
%token REAL

%%
prog    : PROGRAM prog_name VAR dec_list BEGIN_WORD
         stmt_list ENDPERIOD {printf("Rule 1");}
         ;

prog_name   : IDENTIFIER    {;}
            ;

dec_list    : dec  SEMICOLON {;}
            | dec_list dec SEMICOLON {;}
            ;

dec         : dec_id_list COLON type {;}
            ;

type    : INTEGER_KEYWORD {;}
        | REAL_KEYWORD    {;}
        ;

dec_id_list : IDENTIFIER   { printf("Declared identifier: %s\n",$1);}
            | dec_id_list COMMA IDENTIFIER {printf("Declared identifier: %s\n",$3);}
            ;

id_list : IDENTIFIER   {;}
        | id_list COMMA IDENTIFIER {;}
        ;

stmt_list   : stmt {;}
            | stmt_list SEMICOLON stmt {;}
            ;

stmt        : assign {;}
            | read   {;}
            | write  {;}
            | for    {;}
            ;

assign      : IDENTIFIER ASSIGNMENT exp {printf("assignment");}
            ;

exp         : term  {;}
            | exp PLUS term {;}
            | exp MINUS term {;}
            ;
        
term        : factor    {;}
            | term MULTIPLY factor {;}
            | term DIVIDE factor {;}
            ;
        
factor      : IDENTIFIER {;}
            | INTEGER {;}
            | REAL {;}
            | LEFTPARENTHESIS exp RIGHTPARENTHESIS  {;}
            ;

read        : READ LEFTPARENTHESIS id_list RIGHTPARENTHESIS {printf("Read rule");}
            ;

write       : WRITE LEFTPARENTHESIS id_list RIGHTPARENTHESIS {printf("Write rule");}
            ;

for         : FOR index_exp DO body {;}
            ;

index_exp   : IDENTIFIER ASSIGNMENT exp TO exp {printf("assignmentv2");}
            ;

body        : stmt  {;}
            | BEGIN_WORD stmt_list END
%%

int main()
{
    return yyparse();
}

void yyerror(char *s)
{
    printf("%s at line number %d to %d",s,line_num-1, line_num);
}