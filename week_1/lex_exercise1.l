%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
	int count_space, count_line, count_character;
	FILE *fp;
%}

upper [A-Z]
lower [a-z]
digit [0-9]

%%
[" "] {count_space++;}
[\n] {count_line++;}
({lower}|{upper}|{digit}|.) {count_character++;}
%%

int main()
{
	count_space = 0;
	count_line = 0;
	count_character = 0;

	fp = fopen("input_file1.l","r");
	yyin = fp;
	yylex();

	count_space = count_space + count_line;
	printf("Number of lines : %d",count_line);
	printf("\nNumber of words : %d",count_space);
	printf("\nNumber of characters : %d",count_character);

	return 1;
}
