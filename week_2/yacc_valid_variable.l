%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"
%}

letter [A-Za-z]
digit [0-9]
newline [\n]

%%
{letter} {return VAR;}
{digit} {return NUM;}
{newline} {return NEW;}
. {return yytext[0];}
%%

int yywrap()
{
	return 1;
}


