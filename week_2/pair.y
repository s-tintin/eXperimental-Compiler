%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"
	int yylex(void);
	int yyerror(char const *);
%}

%token DIGIT

%%

start : pair '\n' {printf("\nComplete");}
	;

pair: num ',' num {printf("pair(%d,%d)\n",$1,$3); exit(1);}
;
num: DIGIT	{ $$=$1; }
;

%%

int yyerror(char const *s)
{
	printf("Error : %s",s);
}

int main()
{
	yyparse();
	return 1;
}

