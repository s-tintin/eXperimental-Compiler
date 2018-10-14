%{
	#include<stdio.h>
	#include<stdlib.h>

	#define YYSTYPE struct Tnode*

	#include "stage4_p.h"
	#include "stage4_p.c"

	int yylex(void);
	extern int variable[26];
	extern int i;
	int yyerror(char const *);
%}

%token ID NUM ASSGN PLUS MUL EQUAL GREATER LESSER READ WRITE IF THEN ENDIF WHILE DO ENDWHILE NEWLINE
%nonassoc EQUAL GREATER LESSER
%left PLUS
%left MUL


%%

program : slist NEWLINE {evaluate($1); exit(0);}
	;

slist : slist stmt {$$ = TreeCreate(2,6,0,'n',$1,$2);}
	|stmt {$$ = $1;}
	;

stmt : ID ASSGN E ';' {$$ = TreeCreate(0,10,0,'n',$1,$3);}
	|READ '(' ID ')' ';' {$$ = TreeCreate(0,7,0,'n',$3,NULL);}
	|WRITE '(' E ')' ';' {$$ = TreeCreate(0,8,0,'n',$3,NULL);}
	|IF '(' E ')' THEN slist ENDIF ';' {$$ = TreeCreate(2,11,0,'n',$3,$6);}
	|WHILE '(' E ')' DO slist ENDWHILE ';' {$$ = TreeCreate(2,12,0,'n',$3,$6);}
	;

E : E PLUS E {$$ = TreeCreate(0,1,0,'n',$1,$3);}
	|E MUL E {$$ = TreeCreate(0,2,0,'n',$1,$3);}
	|E GREATER E {$$ = TreeCreate(1,4,0,'n',$1,$3);}
	|E LESSER E {$$ = TreeCreate(1,3,0,'n',$1,$3);}
	|E EQUAL E {$$ = TreeCreate(1,5,0,'n',$1,$3);}
	|'(' E ')' {$$ = $2;}
	|ID {$$ = $1;}
	|NUM {$$ = $1;}
	;

%%

int yyerror(char const *s)
{
	printf("\nyyerror : %s\n",s);
	return 0;
}

int main()
{
	for(i=0;i<26;i++)
		variable[i]= -1;

	yyparse();
	return 0;
}
