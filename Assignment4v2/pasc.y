%{
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>
#include "hash_table.h"
void yyerror (char *s);
int yylex();
extern int line_num;
hash_table* declared;
hash_table* initialized;
hash_table* integers;
hash_table* reals;

hash_table* newly_declared;

void check_if_declared(char* identifier);

void check_if_initialized(char* identifier);

void check_type_mismatch(int x, int y);

int find_type(char * identifier);

void add_to_integers();

void add_to_reals();

%}


//Codes corresponding to different tokens
%union {int num;  char arr[256];}         /* Yacc definitions */
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
%type <num> exp term factor
%%
prog    : PROGRAM prog_name VAR dec_list BEGIN_WORD
         stmt_list ENDPERIOD {  
                                    printf("\n\n"); 
                                    printf("Declared variables: ");
                                    print_all_entries(declared); 
                                    printf("Initialized variables: ");
                                    print_all_entries(initialized); 
                                    printf("Intger varaiables: ");
                                    print_all_entries(integers);
                                    printf("Real varaiables: ");
                                    print_all_entries(reals);
                            }
         ;

prog_name   : IDENTIFIER    {;}
            ;

dec_list    : dec  SEMICOLON {;}
            | dec_list dec SEMICOLON {;}
            ;

dec         : dec_id_list COLON type {;}
            ;


dec_id_list : IDENTIFIER    
                        {
                           
                           insert(declared,$1); 
                           insert(newly_declared,$1);
                        }

            | dec_id_list COMMA IDENTIFIER
                        {
                            insert(declared,$3);
                            insert(newly_declared,$3);
                        }
        ;

type        : INTEGER_KEYWORD   { 
                                    add_to_integers();
                                    destroy(newly_declared);
                                    newly_declared = create_new_table(1);
                                }
            | REAL_KEYWORD      { 
                                    add_to_reals();
                                    destroy(newly_declared);
                                    newly_declared = create_new_table(1);
                                }
            ;

id_list : IDENTIFIER    
                        {
                           check_if_declared($1); 
                        }

        | id_list COMMA IDENTIFIER
                        {
                            check_if_declared($3);
                        }
        ;

stmt_list   : stmt {;}
            | stmt_list SEMICOLON stmt {;}
            ;

stmt        : assign {;}
            | read   {;}
            | write  {;}
            | for    {;}
            ;

assign      : IDENTIFIER ASSIGNMENT exp 
                                {
                                    insert(initialized,$1);
                                    check_if_declared($1);
                                    check_type_mismatch(find_type($1),$3);
                                }
            ;

exp         : term  { $$ = $1;}
            | exp PLUS term 
                    {
                        check_type_mismatch($1,$3);
                        $$ = $1;
                    }
            | exp MINUS term 
                    {
                        check_type_mismatch($1,$3);
                        $$ = $1;
                    }
            ;
        
term        : factor    {$$ = $1;}
            | term MULTIPLY factor 
                        {
                            check_type_mismatch($1,$3);
                            $$ = $1;
                        }
            | term DIVIDE factor                         
                        {
                            check_type_mismatch($1,$3);
                            $$ = $1;
                        }

            ;
        
factor      : IDENTIFIER 
                        {
                            check_if_declared($1);
                            check_if_initialized($1);
                            
                            $$ = find_type($1);
                            //printf("Type of %s found: %d\n",$1,find_type($1));
                        }

            | INTEGER 
                        {
                            $$ = 1;
                        }
            | REAL      
                        {
                            $$ = 2;
                        }
            | LEFTPARENTHESIS exp RIGHTPARENTHESIS  
                        {
                            $$ = $2;
                        }
            ;

read        : READ LEFTPARENTHESIS read_id_list RIGHTPARENTHESIS {;}
            ;

read_id_list : IDENTIFIER    
                        {
                           check_if_declared($1); 
                           insert(initialized,$1);
                        }

                | read_id_list COMMA IDENTIFIER
                                {
                                    check_if_declared($3);
                                    insert(initialized,$3);
                                }
                ;

write       : WRITE LEFTPARENTHESIS id_list RIGHTPARENTHESIS {;}
            ;

for         : FOR index_exp DO body {;}
            ;

index_exp   : IDENTIFIER ASSIGNMENT exp TO exp 
                                    {
                                        check_if_declared($1);
                                        insert(initialized,$1);
                                    }
            ;

body        : stmt  {;}
            | BEGIN_WORD stmt_list END
%%

int main()
{
    declared = create_new_table(10);
    initialized = create_new_table(10);
    integers = create_new_table(10);
    reals = create_new_table(10);
    newly_declared = create_new_table(1);
    return yyparse();
}

void yyerror(char *s)
{
    printf("%s at line number %d to %d",s,line_num-1, line_num);
}

void check_if_declared(char *identifier)
{
    int present = is_present(declared, identifier); 
    if(present == 0)
    {
        printf("Error: %s not declared\n",identifier);
    }
}

void check_if_initialized(char *identifier)
{
    int present = is_present(initialized, identifier); 
    if(present == 0)
    {
        printf("Error: %s not initialized\n",identifier);
    }
}

void check_type_mismatch(int x, int y)
{
    if(x == y)
    {
        return;
    }
    printf("Error: Type mismatch detected\n");
}

int find_type(char* identifier)
{
    int is_declared = is_present(declared,identifier);
    if(is_declared == 0)
    {
        return 1;
    }

    int is_integer = is_present(integers,identifier);
    int is_real = is_present(reals,identifier);

    int ret = 0;

    if(is_integer == 1) ret = 1;
    if(is_real == 1) ret = 2;

    //printf("Type of %s is %d\n",identifier,ret);
    return ret;
}

void add_to_integers()
{
    table_entry* e = newly_declared->table[0];
    while(e != NULL)
    {
        insert(integers,e->key);
        e = e->next;
    }
}

void add_to_reals()
{
    table_entry* e = newly_declared->table[0];
    while(e != NULL)
    {
        insert(reals,e->key);
        e = e->next;
    }
}