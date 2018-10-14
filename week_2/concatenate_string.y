%{
	#include<stdio.h>
	#include<stdlib.h>

	#define YYSTYPE char*	
	int yylex(void);
	int yyerror(char const *);
%}

%token WORD NEWLINE

%%
start: WORD NEWLINE WORD NEWLINE {printf("%s + %s",$1,$3); exit(1);}
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
