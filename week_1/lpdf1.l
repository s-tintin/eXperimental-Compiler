%option noyywrap
%{
	#include<stdlib.h>
	#include<stdio.h>
%}

numbers [0-9]+

%%
{numbers}  {printf ("FOUND : %d ", atoi(yytext));}
%%

int main()
{
	yylex();
	return 1;
}
