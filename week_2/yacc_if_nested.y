%{
#include<stdio.h>
#include<stdlib.h>

int yylex(void);
int max(int x,int y);
int yyerror(char const* );

%}

%token IF THEN ELSE BRACKET VAR NUM PLUS MINUS ASSIGN
%left PLUS MINUS
%nonassoc ASSIGN
%%

start : body {printf("levels of nesting: %d",$1); exit(0);}
;

body : IF exp THEN body ELSE body {$$ = max($4,$6) + 1;}
	|IF exp THEN body {$$ = $4 + 1;}
	|exp {$$ = 0;}
;

exp : BRACKET exp BRACKET {$$ = $2;}
	|exp PLUS exp {$$ = 0;}
	|exp MINUS exp {$$ = 0;}
	|exp ASSIGN exp {$$ = 0;}
	|VAR {$$ = 0;}
	|NUM {$$ =0;}
;

%%

int max(int x,int y)
{
	if(x > y)
		return(x);
	else
		return(y);
}

int yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main()
{
	yyparse();
	return 1;
}
