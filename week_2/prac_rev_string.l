%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "y.tab.h"

	char *temp;
%}

%%
[A-Za-z]+ {temp = (char *)malloc(sizeof(yytext)); 
		strcpy(temp,yytext); 
		yylval = temp;
		return WORD;}
[\n] {return NEWLINE;}
. {}
%%

int yywrap()
{
	return 1;
}

