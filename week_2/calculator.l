%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"
%}

%%
[0-9]+ {yylval = atoi(yytext); return DIGIT;}
[+|-|*|/|(|)|\n] {return *yytext;}
. {}
%%

int yywrap()
{
	return 1;
}

