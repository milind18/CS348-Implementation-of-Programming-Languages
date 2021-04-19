%{
    #include "y.tab.h"
    int line_num = 1;

%}

%%
"\n"                    {line_num++;
                        printf("\nLine %d: ",line_num);}
[ \t]+                  {}
"PROGRAM"               {return PROGRAM;
                        }
"VAR"                   {return VAR;}
"BEGIN"                 {return BEGIN_WORD;}
"END"                   {return END;}
"END."                  {return ENDPERIOD;}
"INTEGER"               {return INTEGER_KEYWORD;}
"REAL"                  {return REAL_KEYWORD;}
"FOR"                   {return FOR;}
"READ"                  {return READ;}
"WRITE"                 {return WRITE;}
"TO"                    {return TO;}
"DO"                    {return DO;}
";"                     {return SEMICOLON;}
":"                     {return COLON;}
","                     {return COMMA;}
":="                    {return ASSIGNMENT;}
"+"                     {return PLUS;}
"-"                     {return MINUS;}
"*"                     {return MULTIPLY;}
DIV                     {return DIVIDE;}
"("                     {return LEFTPARENTHESIS;}
")"                     {return RIGHTPARENTHESIS;}
[a-zA-Z]+[a-zA-Z0-9]*   {
                            int len = (int)strlen(yytext);
                            for(int i = 0; i < len; i++)
                            {
                                yylval.arr[i] = yytext[i];
                            }
                            yylval.arr[len] = '\0';
                            return IDENTIFIER;
                        }
[0-9]+                  {  
                            return INTEGER;
                        }
[0-9]+(\.[0-9]+)?(E[+-]?[0-9]+)? {
                                    return REAL;
                                 }
%%


int yywrap(void)
{
    return 1;
}


