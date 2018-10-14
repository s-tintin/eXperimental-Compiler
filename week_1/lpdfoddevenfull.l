%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
%}

numbers [0-9]+

%%
{numbers} {if(atoi(yytext) % 2 == 0) printf("EVEN : %d",yyleng); else printf("ODD : %d",yyleng);}
%%

int main()
{
	yylex();
	return 1;
}
