%{		
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#define YYSTYPE struct tnode*

	#include "stage8.h"
	#include "stage8.c"

	FILE *fp;
	extern FILE *yyin;
	struct tnode* t;
	int yylex(void);
	int yyerror(char const *);
	int a;
	struct Gsymbol *temp;
%}



%token IDENTIFIER READ WRITE NUM NL PLUS MUL ASGN LT GT EQ IF ELSE THEN WHILE DO ENDWHILE ENDIF STMT END BEGINPRO INT ARRAY ARRAYASGN READARRAY DECL ENDDECL MINUS DIV MOD LTE GTE NEQ AND NOT OR BOOL True False
%right ASGN
%left AND OR
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%%

program : declaration body {exit(1);}
	;

declaration : DECL decls ENDDECL {}
	;
	
decls : decls decl {}
	| decl {}
	;

decl : type variables ';' {}
	;

type : INT {a = 0;}
	|BOOL {a = 1;}
	;

variables : variables ',' var {}
	|var {}
	;

var : IDENTIFIER  {if(a == 0) {Ginstall($1->NAME,1,1);}
		else if(a == 1) {Ginstall($1->NAME,2,1);}}
	| IDENTIFIER '[' NUM ']' {if(a == 0) {Ginstall($1->NAME,4,$3->VALUE);}
				else if(a == 1) {Ginstall($1->NAME,5,$3->VALUE);}}
	;

body : BEGINPRO slist END {fp = fopen("inter.xsm","w+"); Generate($2,fp); fclose(fp);
			fp = fopen("inter.xsm","r"); f = fopen("final.xsm","w+");
			finalgen(fp,f); fclose(fp); fclose(f); 
			}
	;

slist : slist stmt {$$ = TreeCreate(TYPE_void,NODE_STMT,0,NULL,$1,$2,NULL);}
	  | stmt  {$$ = $1;}
	  ;

expr : expr PLUS expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_PLUS,$1,$3);
			$$->TYPE = TYPE_int;}
			else {printf("\nTYPE ERROR ADD"); exit(0);}}

	| expr MUL expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_MUL,$1,$3);
			$$->TYPE = TYPE_int;} 
			else {printf("\nTYPE ERROR MUL"); exit(0);}}

	| expr MINUS expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
				{$$ = makeOperatorNode(NODE_MINUS,$1,$3);
				$$->TYPE = TYPE_int;}
				else{printf("\nTYPE ERROR MINUS"); exit(0);}}

	| expr DIV expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_DIV,$1,$3);
			$$->TYPE = TYPE_int;}
			else {printf("\nTYPE ERROR DIV"); exit(0);}}

	| expr MOD expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int)))  
			{$$ = makeOperatorNode(NODE_MOD,$1,$3);
			$$->TYPE = TYPE_int;} 
			else {printf("\nTYPE ERROR MOD"); exit(0);}}
			
	| expr LT expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int)))  
			{ $$ = makeOperatorNode(NODE_LT,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR LESS THAN"); exit(0);}}
			
	| expr GT expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int)))  
			{$$ = makeOperatorNode(NODE_GT,$1,$3);
			$$->TYPE = TYPE_bool;} 
			else {printf("\nTYPE ERROR GREATER THAN"); exit(0);}}

	| expr EQ expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int)))  
			{$$ = makeOperatorNode(NODE_EQ,$1,$3);
			$$->TYPE = TYPE_bool;}
			else{ printf("\nTYPE ERROR EQUAL"); exit(0);}}

	| expr NEQ expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_NEQ,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR NOT EQUAL"); exit(0);}}
			
	| expr LTE expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_LTE,$1,$3);
			$$->TYPE = TYPE_bool;}
			else { printf("\nTYPE ERROR LESS THAN OR EQUAL"); exit(0);}}

	| expr GTE expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NODE_GTE,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR GREATER THAN OR EQUAL"); exit(0);}}
			
	| expr AND expr  {if(($1->TYPE != TYPE_bool) || ($3->TYPE != TYPE_bool)) { printf("\nTYPE ERROR AND"); exit(0);}
			$$ = makeOperatorNode(NODE_AND,$1,$3);
			$$->TYPE = TYPE_bool;}

	| expr OR expr  {if(($1->TYPE != TYPE_bool) || ($3->TYPE != TYPE_bool)) { printf("\nTYPE ERROR OR"); exit(0);}
			$$ = makeOperatorNode(NODE_OR,$1,$3);
			$$->TYPE = TYPE_bool;}

	| NOT expr  {if($2->TYPE != TYPE_bool) { printf("\nTYPE ERROR NOT"); exit(0);}
			$$ = makeOperatorNode(NODE_NOT,$2,NULL);
			$$->TYPE = TYPE_bool;}

	| '(' expr ')' {$$ = $2;}

	| NUM {$$ = $1;
		$$->TYPE = TYPE_int;}

	| IDENTIFIER {temp = Gsearch($1->NAME);
		if(temp == NULL)
     		{
     			printf("\nVariable undeclared");
     			exit(0);
     		}
	
		$$->TYPE = temp->type;
		$1->Gentry = temp;
                $$ = $1;}

 	| IDENTIFIER '[' expr ']' {temp = Gsearch($1->NAME);
				if(temp == NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(0);
		     		}
			 	$$ = TreeCreate(TYPE_void,NODE_ARRAY,0,$1->NAME,$1,$3,NULL);
				$$->TYPE = temp->type;
				$1->Gentry = temp;}
	|True {$$=$1;
		$$->TYPE = TYPE_bool;}

	|False {$$=$1;
		$$->TYPE = TYPE_bool;}
	;

stmt : IDENTIFIER ASGN expr ';' {temp = Gsearch($1->NAME);
			if(temp == NULL)
		     	{
		     		printf("\nVariable undeclared");
		     		exit(0);
		     	}
			$1->Gentry = temp;

			if((((temp->type) == TYPE_int) && ($3->TYPE == TYPE_int)) ||  (((temp->type) == TYPE_int) && ($3->TYPE == TYPE_intarray)))
			{$$=TreeCreate(TYPE_void,NODE_ASGN,0,NULL,$1,$3,NULL);}

			else if((((temp->type) == TYPE_bool) && ($3->TYPE == TYPE_bool)) ||  (((temp->type) == TYPE_bool) && ($3->TYPE == TYPE_boolarray)))
			{$$=TreeCreate(TYPE_void,NODE_ASGN,0,NULL,$1,$3,NULL);}

			else
			{printf("TYPE ERROR ASGN"); exit(0);}}

	| READ '(' IDENTIFIER ')'';' {temp = Gsearch($3->NAME);
				if(temp == NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(0);
		     		}
				$3->Gentry = temp;
				if((temp->type) != TYPE_int) {printf("TYPE ERROR READ"); exit(0);}
				
				$$=TreeCreate(TYPE_void,NODE_READ,0,NULL,$3,NULL,NULL);}

	| WRITE '('expr ')' ';' {if(($3->TYPE == TYPE_int) || ($3->TYPE == TYPE_intarray)) {$$=TreeCreate(TYPE_void,NODE_WRITE,0,NULL,$3,NULL,NULL);}
				else {printf("TYPE ERROR WRITE"); exit(0);}}
				
	| IF '(' expr ')' THEN slist ENDIF ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR IF"); exit(0);}
						 $$=TreeCreate(TYPE_void,NODE_IF,0,NULL,$3,$6,NULL);}

	| IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void) || ($8->TYPE != TYPE_void)) {printf("TYPE ERROR IF"); exit(0);}
						 $$=TreeCreate(TYPE_void,NODE_ELSE,0,NULL,$3,$6,$8);}

	| WHILE '(' expr ')' DO slist ENDWHILE  ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR WHILE"); exit(0);}
						$$=TreeCreate(TYPE_void,NODE_WHILE,0,NULL,$3,$6,NULL);}
	
	| IDENTIFIER '[' expr ']' ASGN expr ';' {temp = Gsearch($1->NAME);

					if(temp == NULL)
				     		{
				     			printf("\nVariable undeclared");
				     			exit(0);
				     		}

					$1->Gentry = temp;
					if((((temp->type) == TYPE_intarray) && ($6->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) ||  (((temp->type) == TYPE_intarray) && ($6->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)))
						{$$ = TreeCreate(TYPE_void,NODE_ARRAYASGN,0,NULL,$1,$3,$6);}

					else if((((temp->type) == TYPE_boolarray) && ($3->TYPE == TYPE_bool)) || (((temp->type) == TYPE_boolarray) && ($3->TYPE == TYPE_boolarray)))
						{$$ = TreeCreate(TYPE_void,NODE_ARRAYASGN,0,NULL,$1,$3,$6);}

					else
					{printf("TYPE ERROR ASGN"); exit(0);}}

	| READ '(' IDENTIFIER '[' expr ']' ')' ';' {temp = Gsearch($3->NAME);
						if(temp == NULL)
				     		{
				     			printf("\nVariable undeclared");
				     			exit(0);
				     		}
					
						$3->Gentry = temp;
						if(((temp->type) == TYPE_intarray) && ($5->TYPE == TYPE_int)){ 
						$$ = TreeCreate(TYPE_void,NODE_READARRAY,0,NULL,$3,$5,NULL);}

						else {printf("TYPE ERROR READ"); exit(0);}}
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s", s);
}
int main()
{ 
	fp=fopen("mod_bubblesort.txt","r");
	if(fp)
		yyin=fp;
   
    	yyparse(); 
    	return 1; 
}
