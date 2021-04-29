%{
    #include <bits/stdc++.h>
    #include "codes.h"
    #include "hash_table.h"
    using namespace std;

    extern struct table_entry* install_id(char *str);
    extern struct table_entry* install_num(char *str);

    vector < list <table_entry> > symbol_table(NSLOTS);
    struct table_entry* insert(table_entry a);
    int line_num = 1;

%}

%%
"\n"                    {line_num++;}
[ \t]+                  {}
"PROGRAM"               {return PROGRAM_code;
                        }
"VAR"                   {return VAR_code;}
"BEGIN"                 {return BEGIN_code;}
"END"                   {return END_code;}
"END."                  {return ENDPERIOD_code;}
"INTEGER"               {return INTEGER_KEYWORD_code;}
"FOR"                   {return FOR_code;}
"READ"                  {return READ_code;}
"WRITE"                 {return WRITE_code;}
"TO"                    {return TO_code;}
"DO"                    {return DO_code;}
";"                     {return SEMICOLON_code;}
":"                     {return COLON_code;}
","                     {return COMMA_code;}
":="                    {return ASSIGNMENT_code;}
"+"                     {return PLUS_code;}
"-"                     {return MINUS_code;}
"*"                     {return MULTIPLY_code;}
DIV                     {return DIVIDE_code;}
"("                     {return LEFTPARENTHESIS_code;}
")"                     {return RIGHTPARENTHESIS_code;}
[a-zA-Z]+[a-zA-Z0-9]*   {
                            install_id(yytext);
                            return IDENTIFIER_code;
                        }
[0-9]+                  {   install_num(yytext);
                            return INTEGER_code;
                        }
%%


struct table_entry* insert(table_entry x)
{
    int slot_num = compute_hash(x.a);
    //printf("Inserting into slot : %d\n", slot_num);
    symbol_table[slot_num].push_back(x);
    auto itr = symbol_table[slot_num].rbegin();
    auto ptr = &(*itr);
    return ptr;
    
}
struct table_entry* install_id(char* a)
{
    //printf("Identifier: %s \n",a);
    return insert(table_entry(a));
}

struct table_entry* install_num(char *a)
{
    //printf("Number: %s \n", a);
    return insert(table_entry(a));
}

int yywrap(void)
{
    return 1;
}


