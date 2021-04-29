#include <bits/stdc++.h>
#include "codes.h"
using namespace std;
extern int yylex();
extern int yylineno();
extern int line_num;
extern char * yytext;

//extern vector< list <table_entry> > symbol_table;
int main()
{
    printf("Line        Token Type      Token Specifier\n");
    int ntoken;
    ntoken = yylex();
    while(ntoken)
    {
        printf("%d              %d", line_num, ntoken);
        if(ntoken == INTEGER_code)
        {
            printf("             #%s\n",yytext);
        }
        else if(ntoken == IDENTIFIER_code)
        {
            printf("             ^%s\n",yytext);
        }
        else
        {
            printf("\n");
        }
        ntoken = yylex();
    }
    
    //printf("%d \n", (int)symbol_table.size());
    return 0;
}
