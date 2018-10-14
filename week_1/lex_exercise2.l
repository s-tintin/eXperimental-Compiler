%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
	int num_int , num_float;
	FILE *fp;
%}

digit [0-9]
sign [+|-]
exponent [E]

%%
({sign}?{digit}+) {num_int++;}
({sign}?{digit}+)(.{digit}+)?({exponent}{sign}?{digit}+)? {num_float++;}
[ |\n|\t] {}
. 
%%

int main()
{
	num_int = 0;
	num_float = 0;
	fp = fopen("input_file2.l","r");
	yyin = fp;
	yylex();
	
	printf("\nNumber of integers : %d",num_int);
	printf("\nNumber of floating point numbers : %d\n",num_float);

	return 1;
}
