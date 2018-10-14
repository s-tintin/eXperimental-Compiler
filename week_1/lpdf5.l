%{
	#include<stdlib.h>
	#include<stdio.h>
	char *file1;
%}

numbers [0-9]+

%%
{numbers}  {printf ("FOUND : %d ", atoi(yytext));}
%%

int yywrap()
{
	char *file2;
	file2 = "second";
	if(strcmp(file1,file2) != 0)
	{
		file1 = file2;
		yyin = stdin;
		return 0;
	}
		
	else
		return 1;
}

int main()
{
	file1 = "first";
	yylex();
	return 1;
}
