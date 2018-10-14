%option noyywrap
%{

	#include "y.tab.h"
	#include<stdlib.h>
	#include<stdio.h>
	#include<string.h>

	char *ptr;

%}

letter [A-Za-z]+
newline [\n]

%%

{letter} {
	  ptr = malloc(sizeof (yytext));
	  strcpy(ptr,yytext);
	  yylval = ptr;
          return WORD;}
{newline} {return NEWLINE;}
.	{return *yytext;}

%%


