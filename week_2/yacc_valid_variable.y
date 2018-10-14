%{
	#include<stdio.h>
	#include<stdlib.h>
	
	int yylex(void);
	int yyerror(char const*);
%}

%token VAR NUM NEW

%%
start : expr NEW {printf("\nVALID VARIABLE\n"); exit(0);}
      ;
expr : VAR rest
     ;
rest : VAR rest | NUM rest | VAR | NUM
     ;
%%

int yyerror(char const *s)
{
	printf("yyerror %s",s);
	exit(0);
}

int main()
{
	printf("\nEnter the identifier: ");
	yyparse();
	return 1;
}
