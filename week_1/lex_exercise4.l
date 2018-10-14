%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
	FILE *fp;
%}

single \/\/.*
multiple \/\*.[^"*/"]*\*\/

%%
{single} {printf("\nThis is a single line comment %s",yytext);}
{multiple} {printf("\nThis is a multiple line comment %s",yytext);}
[ |\n|\t] {}
. {}
%%

int main()
{
	fp = fopen("input_file4.l","r");
	yyin = fp;
	yylex();
	
	return 1;
}  
