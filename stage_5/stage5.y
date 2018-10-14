%{		
	#define YYSTYPE struct tnode*

	#include <stdio.h>
	#include <stdlib.h>
	#include "stage5.h"

	FILE *fp;
	extern FILE *yyin;
	struct tnode* t;
	int yylex(void);
	int yyerror(char const *);
	struct Lsymbol *temp,*temp1;
%}



%token ID READ WRITE NUM NL PLUS MUL ASGN LT GT EQ IF ELSE THEN WHILE DO ENDWHILE ENDIF STMT END BEGINPRO INT ARRAY ARRAYASGN READARRAY DECL ENDDECL
%nonassoc LT GT EQ
%left PLUS
%left MUL
%%

program : declaration body {}
	;

declaration : DECL decls ENDDECL {}
	;

decls : decls decl {}
	| decl {}
	;

body : BEGINPRO slist END {evaluate($2); exit(1);}
	;
decl : INT ID ';'  {Linstall($2->NAME,1,1);}
	| INT ID '[' NUM ']' ';'  {Linstall($2->NAME,2,$4->VALUE);}
	;
slist : slist stmt {$$ = TreeCreate(-10,STMT,0,NULL,$1,$2);}
	  | stmt  {$$=$1;}
	  ;
expr : expr PLUS expr	{$$ = makeOperatorNode(PLUS,$1,$3);}
	 | expr MUL expr  	{$$ = makeOperatorNode(MUL,$1,$3);}
	 | expr LT expr  	{$$ = makeOperatorNode(LT,$1,$3);}
	 | expr GT expr  	{$$ = makeOperatorNode(GT,$1,$3);}
	 | expr EQ expr  	{$$ = makeOperatorNode(EQ,$1,$3);}
	 | '(' expr ')'		{$$ = $2;}
	 | NUM			{$$ = $1;}
	 | ID               	{$$ = $1;}
	 | ID '[' NUM ']'       {$$ = TreeCreate(-10,ARRAY,0,$1->NAME,$1,$3);}
	 | ID '[' ID ']' 	{$$ = TreeCreate(-10,ARRAY,0,$1->NAME,$1,$3);}
	 ;
stmt : ID ASGN expr ';'  {$$=TreeCreate(-10,ASGN,0,NULL,$1,$3);}
	 | READ '(' ID ')'';' {$$=TreeCreate(-10,READ,0,NULL,$3,NULL);}
	 | WRITE '('expr ')' ';' {$$=TreeCreate(-10,WRITE,0,NULL,$3,NULL);}
	 | IF '(' expr ')' THEN slist ENDIF ';' {$$=TreeCreate(-10,IF,0,NULL,$3,$6);}
	 | WHILE '(' expr ')' DO slist ENDWHILE  ';' {$$=TreeCreate(-10,WHILE,0,NULL,$3,$6);}	
	 | ID '[' NUM ']' ASGN expr ';' {$$ = TreeCreate(-10,ARRAYASGN,0,$1->NAME,$3,$6);}
	 | ID '[' ID ']' ASGN expr ';' {$$ = TreeCreate(-10,ARRAYASGN,0,$1->NAME,$3,$6);}
	 | READ '(' ID '[' NUM ']' ')' ';' {$$ = TreeCreate(-10,READARRAY,0,NULL,$3,$5);}
	 | READ '(' ID '[' ID ']' ')' ';' {$$ = TreeCreate(-10,READARRAY,0,NULL,$3,$5);}
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s", s);
}

int main()
{
	fp=fopen("in5.txt","r");
	if(fp)
		yyin=fp; 
	yyparse();
	return 1;
}
