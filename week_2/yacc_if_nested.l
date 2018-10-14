%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"
%}

letter [A-Za-z]
digit [0-9]+
IF [i][f]
THEN [t][h][e][n]
ELSE [e][l][s][e]
bracket [()]

%%
{letter} {return VAR;}
{digit} {return NUM;}
{IF} {return IF;}
{THEN} {return THEN;}
{ELSE} {return ELSE;}
{bracket} {return BRACKET;}
[-] {return MINUS;}
[+] {return PLUS;}
[=] {return ASSIGN;}
. {}
%%

int yywrap()
{
	return 1;
}


