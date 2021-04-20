/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAM = 258,
    VAR = 259,
    BEGIN_WORD = 260,
    END = 261,
    ENDPERIOD = 262,
    INTEGER_KEYWORD = 263,
    REAL_KEYWORD = 264,
    FOR = 265,
    READ = 266,
    WRITE = 267,
    TO = 268,
    DO = 269,
    SEMICOLON = 270,
    COLON = 271,
    COMMA = 272,
    ASSIGNMENT = 273,
    PLUS = 274,
    MINUS = 275,
    MULTIPLY = 276,
    DIVIDE = 277,
    LEFTPARENTHESIS = 278,
    RIGHTPARENTHESIS = 279,
    IDENTIFIER = 280,
    INTEGER = 281,
    REAL = 282
  };
#endif
/* Tokens.  */
#define PROGRAM 258
#define VAR 259
#define BEGIN_WORD 260
#define END 261
#define ENDPERIOD 262
#define INTEGER_KEYWORD 263
#define REAL_KEYWORD 264
#define FOR 265
#define READ 266
#define WRITE 267
#define TO 268
#define DO 269
#define SEMICOLON 270
#define COLON 271
#define COMMA 272
#define ASSIGNMENT 273
#define PLUS 274
#define MINUS 275
#define MULTIPLY 276
#define DIVIDE 277
#define LEFTPARENTHESIS 278
#define RIGHTPARENTHESIS 279
#define IDENTIFIER 280
#define INTEGER 281
#define REAL 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 32 "pasc.y" /* yacc.c:1909  */
int num;  char arr[256];

#line 111 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
