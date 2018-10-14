%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>

	#define YYSTYPE struct Tnode*

	#include "stage5_p.h"
	#include "stage5_p.c"

	int yylex(void);
	int yyerror(char const *);
%}

%token ID NUM ASSGN PLUS MUL EQUAL GREATER LESSER READ WRITE IF THEN ENDIF WHILE DO ENDWHILE NEWLINE DECL ENDDECL START END INT
%nonassoc EQUAL GREATER LESSER 
%left PLUS
%left MUL


%%

program : declaration main {}
	;

declaration : DECL dcls ENDDECL {}
	;

dcls : dcls decl {}
	| decl {}
	;

decl : INT ID ';' {Linstall($2->name,0,1);}

	| INT ID '[' NUM ']' ';' {Linstall($2->name,3,$4->value);}
	;

main : START slist END {printf("\nEVALUATION"); evaluate($2); exit(0);}
	;
				
slist : slist stmt {$$ = TreeCreate(2,6,0,NULL,$1,$2);}
	|stmt {$$ = $1;}
	;

E : E PLUS E {$$ = TreeCreate(0,1,0,NULL,$1,$3);}

	|E MUL E {$$ = TreeCreate(0,2,0,NULL,$1,$3);}

	|E GREATER E {$$ = TreeCreate(1,4,0,NULL,$1,$3);}

	|E LESSER E {$$ = TreeCreate(1,3,0,NULL,$1,$3);}

	|E EQUAL E {$$ = TreeCreate(1,5,0,NULL,$1,$3);}

	|'(' E ')' {$$ = $2;}

	|ID {if(Llookup($1->name) == NULL)
		{
			printf("\nVariable Undeclared");
			exit(0);
		}
		$$ = $1;}

	|NUM {printf("NUMBER");$$ = $1;}

	|ID '[' NUM ']' {if(Llookup($1->name) == NULL)
			{
				printf("\nVariable Undeclared");
				exit(0);
			}
			$$ = TreeCreate(2,13,0,NULL,$1,$3);}

	|ID '[' ID ']' {if(Llookup($1->name) == NULL)
			{
				printf("\nVariable Undeclared");
				exit(0);
			}
			$$ = TreeCreate(2,13,0,NULL,$1,$3);}
	;


stmt : ID ASSGN E ';' {printf("\ngdihijoj");
			if(Llookup($1->name) == NULL)
			{printf("\ngdihijoj");
				printf("\nVariable Undeclared");
				exit(0);
			}
			printf("\ngdihijoj");		
			$$ = TreeCreate(2,10,0,NULL,$1,$3);}

	| ID '[' NUM ']' ASSGN E ';' {if(Llookup($1->name) == NULL)
					{
						printf("\nVariable Undeclared");
						exit(0);
					}		
					$$ = TreeCreate(2,10,0,$1->name,$3,$6);}

	| ID '[' ID ']' ASSGN E ';' {if(Llookup($1->name) == NULL)
					{
						printf("\nVariable Undeclared");
						exit(0);
					}		
					$$ = TreeCreate(2,10,0,$1->name,$3,$6);}

	| READ '(' ID ')' ';' {if(Llookup($3->name) == NULL)
				{
					printf("\nVariable Undeclared");
					exit(0);
				}
				printf("\nREAD");		
				$$ = TreeCreate(2,7,0,NULL,$3,NULL);}

	| READ '(' ID '[' ID ']' ')' ';' {if(Llookup($3->name) == NULL)
					{
						printf("\nVariable Undeclared");
						exit(0);
					}
				$$ = TreeCreate(2,15,0,NULL,$3,$5);}

	| READ '(' ID '[' NUM ']' ')' ';' {if(Llookup($3->name) == NULL)
					{
						printf("\nVariable Undeclared");
						exit(0);
					}
				$$ = TreeCreate(2,15,0,NULL,$3,$5);}
			

	|WRITE '(' E ')' ';' {printf("\nWRITE"); $$ = TreeCreate(2,8,0,NULL,$3,NULL);}

	|IF '(' E ')' THEN slist ENDIF ';' {$$ = TreeCreate(2,11,0,NULL,$3,$6);}

	|WHILE '(' E ')' DO slist ENDWHILE ';' {$$ = TreeCreate(2,12,0,NULL,$3,$6);}
	;


%%

int yyerror(char const *s)
{
	printf("\nyyerror : %s\n",s);
	return 0;
}

int main()
{
	head = NULL;
	yyparse();
	return 0;
}
