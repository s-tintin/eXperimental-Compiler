%{
	#include<stdlib.h>
	#include<stdio.h>
%}

numbers [0-9]+

%%
{numbers}  {printf ("FOUND : %d ", atoi(yytext));}
%%

int yywrap()
{
	if(yyin != stdin)
	{
		yyin = stdin;
		return 0;
	}
		
	else
		return 1;
}

int main()
{
	FILE * newfile_pointer;
	newfile_pointer = fopen("input_file.l","r");
	yyin = newfile_pointer;

	yylex();
	return 1;
}
