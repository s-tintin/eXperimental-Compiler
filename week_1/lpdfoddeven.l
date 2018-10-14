%option noyywrap
%{
	#include<stdlib.h>
	#include<stdio.h>
%}

numbers [0-9]+

%%
{numbers}  {if (((atoi(yytext)) % 2) == 0) printf("EVEN"); else printf("ODD");}
%%

int main()
{
	yylex();
	return 1;
}
