%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
	FILE *fp;
%}
letter [a-zA-Z]+

%%
{letter} {if(yyleng == 3 && yytext[0] == 'A' ) printf("%s ",yytext);}
[ |\n|\t] {}
. 
%%

int main()
{
	fp = fopen("input_file3.l","r");
	yyin = fp;
	yylex();
	
	return 1;
}
