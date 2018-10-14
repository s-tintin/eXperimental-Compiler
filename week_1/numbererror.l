%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
%}
	
number [0-9]+
 
%%
{number} {printf("Found : %d\n",atoi(yytext));}
[ \n\t] {}
. { yyerror("Invalid charater "); printf("%s\n",yytext); exit(1); }
%%

int yyerror(char const *s)
{
	printf("yyerror %s",s);
	return 1;
}
 
int main()
{
	FILE *fp;
	fp = fopen("inputfile.l","r");
	yyin = fp;
	yylex();
	return 1;
}
