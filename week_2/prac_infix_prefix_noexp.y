%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	
	#define YYSTYPE char*

	int yylex(void);
	int yyerror(char const* );
%}


%token DIGIT NEWLINE
%left '+' '-'
%left '*' '/'

%%
start : expr NEWLINE {$$ = $1; printf("%s",$1); exit(1);}
;
expr: expr '+' expr {char *temp; temp = (char *)malloc(3); strcpy(temp,"+"); $$ = strcat(temp,(strcat($1,$3)));}
	|expr '-' expr {char *temp; temp = (char *)malloc(3); strcpy(temp,"-"); $$ = strcat(temp,(strcat($1,$3)));}
	|expr '*' expr {char *temp; temp = (char *)malloc(3); strcpy(temp,"*"); $$ = strcat(temp,(strcat($1,$3)));}
	|expr '/' expr {char *temp; temp = (char *)malloc(3); strcpy(temp,"/"); $$ = strcat(temp,(strcat($1,$3)));}
    | DIGIT        { $$=$1; }
    ;
%%

int yyerror(char const *s)
{
	printf("Error : %s", s);
}
	
int main()
{
	yyparse();
	return 1;
}
