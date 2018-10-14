%{
	#include<stdio.h>
	#include<stdlib.h>

	#define YYSTYPE char*	
	int yylex(void);
	int yyerror(char const *);
%}

%token WORD NEWLINE
%left '+'
%left '*'

%%
start: expr NEWLINE	{exit(1);}
     ;
expr: expr '+' expr	{printf("+ ");}
    | expr '*' expr	{printf("* ");}
    | '(' expr ')'
    | WORD		{printf("%s ",$1);}
    ;
%%

int yyerror(char const *s)
{
	printf("yyerror : %s",s);
}

int main()
{
	yyparse();
	return 0;
}