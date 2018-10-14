%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	
	int yylex(void);
	int yyerror(char const* );

	#define YYSTYPE char*
%}

%token WORD NEWLINE

%%
start : expr NEWLINE {printf("\n%s",$1); exit(1);}
;
expr : expr WORD {char *temp; temp = strcat((strcat($2," ")),$1); $$ = temp;}
	|WORD {$$ = $1;}
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
