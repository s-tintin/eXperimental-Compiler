%option noyywrap
%{

	#include "y.tab.h"
	#include<stdlib.h>
	#include<stdio.h>

%}

letter [A-Za-z]
newline [\n]

%%

{letter} {yylval = *yytext;
          return VAR;}
{newline} {return NEWLINE;}
.	return *yytext;

%%


