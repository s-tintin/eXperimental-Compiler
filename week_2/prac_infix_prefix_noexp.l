%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"

	char *ptr;
%}

%%
[0-9]+ {ptr = (char *)malloc(sizeof (yytext));
	  strcpy(ptr,yytext);
	  yylval = ptr;
		return DIGIT;}
[+|-|*|/] {return *yytext;}
[\n] {return NEWLINE;}
. {}
%%

int yywrap()
{
	return 1;
}
