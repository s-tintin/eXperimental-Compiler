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
	FILE *newfile_pointer;
	newfile_pointer = fopen("input_file.l","r");
	char *file2 = "input_file.l";

	if(strcmp (file1,file2) != 0)
	{
		file1 = file2;
		yyin = newfile_pointer;
		return 0;
	}
		
	else
		return 1;
}

int main()
{
	file1 = "stdin";
	yyin = stdin;
	yylex();
	return 1;
}
