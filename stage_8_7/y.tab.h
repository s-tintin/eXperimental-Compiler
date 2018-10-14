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
    IDENTIFIER = 258,
    READ = 259,
    WRITE = 260,
    NUM = 261,
    NL = 262,
    PLUS = 263,
    MUL = 264,
    ASGN = 265,
    LT = 266,
    GT = 267,
    EQ = 268,
    IF = 269,
    ELSE = 270,
    THEN = 271,
    WHILE = 272,
    DO = 273,
    ENDWHILE = 274,
    ENDIF = 275,
    STMT = 276,
    END = 277,
    BEGINPRO = 278,
    INT = 279,
    ARRAY = 280,
    ARRAYASGN = 281,
    READARRAY = 282,
    DECL = 283,
    ENDDECL = 284,
    MINUS = 285,
    DIV = 286,
    MOD = 287,
    LTE = 288,
    GTE = 289,
    NEQ = 290,
    AND = 291,
    NOT = 292,
    OR = 293,
    BOOL = 294,
    True = 295,
    False = 296
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define READ 259
#define WRITE 260
#define NUM 261
#define NL 262
#define PLUS 263
#define MUL 264
#define ASGN 265
#define LT 266
#define GT 267
#define EQ 268
#define IF 269
#define ELSE 270
#define THEN 271
#define WHILE 272
#define DO 273
#define ENDWHILE 274
#define ENDIF 275
#define STMT 276
#define END 277
#define BEGINPRO 278
#define INT 279
#define ARRAY 280
#define ARRAYASGN 281
#define READARRAY 282
#define DECL 283
#define ENDDECL 284
#define MINUS 285
#define DIV 286
#define MOD 287
#define LTE 288
#define GTE 289
#define NEQ 290
#define AND 291
#define NOT 292
#define OR 293
#define BOOL 294
#define True 295
#define False 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
