%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();
%}
%%
"print"				   {return print;}
"exit"				   {return exit_command;}
[a-zA-Z]+[a-zA-Z0-9]*  {int len = (int)strlen(yytext);
                        for(int i = 0; i < len; i++)
                        {
                            yylval.arr[i] = yytext[i];
                        }
                        return identifier;}
[0-9]+                 {yylval.num = atoi(yytext); return number;}
[ \t\n]                ;
[-+=;]           	   {return yytext[0];}
.                      {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}
