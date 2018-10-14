%{
/*** Auxiliary declarations section ***/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int yylex(void);
int yyerror(char const *);

%}

 /*** YACC Declarations section ***/
%token DIGIT
%left '+' '-'
%left '*' '/'
%%

/*** Rules Section ***/
start : expr '\n'		{printf("The value is : %d\n",$1); exit(1);}
      ;

expr: expr '+' expr     {$$ = $1 + $3;}
    | expr '*' expr     {$$ = $1 * $3;}
    | expr '-' expr     {$$ = $1 - $3;}
    | expr '/' expr     {$$ = $1 / $3;}
    | '(' expr ')'	{$$ = $2;}
    | DIGIT             {$$ = $1;}
    ;

%%


int yyerror(char const *s)
{
    printf("yyerror %s",s);
}

int yylex(){
   int c;
    c = getchar();
    if(isdigit(c)){
        yylval = c - '0';
        return DIGIT;
    }
    else if(c == ' '){
        yylex();         /*This is to ignore whitespaces in the input*/
    }
    else {
        return c;
    }
}

int main()
{
	yyparse();
	return 1;
}
