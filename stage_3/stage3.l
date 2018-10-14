%option noyywrap

%{
	#include "y.tab.h"
	#include<stdio.h>
	#include<stdlib.h>
%}

%%

[a-z] {yylval.character = *yytext; return ID;}
[0-9] {yylval.integer = atoi(yytext); return NUMBER;}
"read" {return READ;}
"write" {return WRITE;}
[()] {return *yytext;}
"=" {return ASSIGN;}
"+" {return PLUS;}
"*" {return MUL;}
";" {return SEMICOLON;}
[\n] {return END;}
[\t] {}
. {}

%%
