%{
	#include<stdio.h>
	#include<stdlib.h>
	
	int yylex(void);
	void yyerror(char const *);
	int variable[26];
	int x;
%}

%union
{
	int integer;
	char character;
};

%token ID NUMBER READ WRITE SEMICOLON END ASSIGN PLUS MUL
%left PLUS
%left MUL
%type <integer> NUMBER
%type <character> ID
%type <integer> E

%%

prog: slist END {exit(1);};

slist: slist stmt {}
     | stmt {};

stmt: ID ASSIGN E SEMICOLON {variable[$1 - 'a'] = $3;}
	|READ '(' ID ')' SEMICOLON  {scanf("%d",&x); variable[$3 - 'a'] = x;}
	|WRITE '(' E ')' SEMICOLON {printf("%d\n",$3);};

E: E PLUS E {$$ = $1 + $3;}
	|E MUL E {$$ = $1 * $3;}
	|'(' E ')' {$$ = $2;}
	|NUMBER {$$ = $1;}
	|ID {$$ = variable[$1 - 'a'];};

%%

void yyerror(char const *s)
{
	printf("yyerror : %s",s);
}

int main()
{
	yyparse();
	return 0;
}
