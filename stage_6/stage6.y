%{		
	#define YYSTYPE struct tnode*

	#include <stdio.h>
	#include <stdlib.h>
	#include "stage6.h"

	FILE *fp;
	extern FILE *yyin;
	struct tnode* t;
	int yylex(void);
	int yyerror(char const *);
	int a;
	struct Lsymbol *temp;
%}



%token ID READ WRITE NUM NL PLUS MUL ASGN LT GT EQ IF ELSE THEN WHILE DO ENDWHILE ENDIF STMT END BEGINPRO INT ARRAY ARRAYASGN READARRAY DECL ENDDECL MINUS DIV MOD LTE GTE NEQ AND NOT OR BOOL True False
%right ASGN
%left AND OR
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT

%%

program : declaration body {exit(0);}
	;

declaration : DECL decls ENDDECL {}
	;
	
decls : decls decl {}
	| decl {}
	;

decl : type variables ';' {}
	;

type : INT {a=0;}
	|BOOL {a=1;}
	;

variables : variables ',' var {}
	|var {}
	;

var : ID  {if(a==0) {Linstall($1->NAME,1,1);}
		else if(a==1) {Linstall($1->NAME,2,1);}}
	| ID '[' NUM ']' {if(a==0) {Linstall($1->NAME,4,$3->VALUE);}
				else if(a==1) {Linstall($1->NAME,5,$3->VALUE);}}
	;

body : BEGINPRO slist END {evaluate($2);}
	;

slist : slist stmt {$$ = TreeCreate(TYPE_void,STMT,0,NULL,$1,$2);}
	  | stmt  {$$=$1;}
	  ;

expr : expr PLUS expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(PLUS,$1,$3);
				$$->TYPE = TYPE_int;}
			else {printf("\nTYPE ERROR ADD"); exit(0);}}

	| expr MUL expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(MUL,$1,$3);
			$$->TYPE = TYPE_int;} 
			else {printf("\nTYPE ERROR MUL"); exit(0);}}

	| expr MINUS expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
				{$$ = makeOperatorNode(MINUS,$1,$3);
				$$->TYPE = TYPE_int;}
				else{printf("\nTYPE ERROR MINUS"); exit(0);}}

	| expr DIV expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(DIV,$1,$3);
			$$->TYPE = TYPE_int;}
			else {printf("\nTYPE ERROR DIV"); exit(0);}}

	| expr MOD expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(MOD,$1,$3);
			$$->TYPE = TYPE_int;} 
			else {printf("\nTYPE ERROR MOD"); exit(0);}}
			
	| expr LT expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(LT,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR LESS THAN"); exit(0);}}
			

	| expr GT expr {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(GT,$1,$3);
			$$->TYPE = TYPE_bool;} 
			else {printf("\nTYPE ERROR GREATER THAN"); exit(0);}}

	| expr EQ expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(EQ,$1,$3);
			$$->TYPE = TYPE_bool;}
			else{printf("\nTYPE ERROR EQUAL"); exit(0);}}

	| expr NEQ expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(NEQ,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR NOT EQUAL"); exit(0);}}
			
	| expr LTE expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int)))  
			{$$ = makeOperatorNode(LTE,$1,$3);
			$$->TYPE = TYPE_bool;}
			else { printf("\nTYPE ERROR LESS THAN OR EQUAL"); exit(0);}}

	| expr GTE expr  {if((($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_int) && ($3->TYPE == TYPE_intarray)) || (($1->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_int))) 
			{$$ = makeOperatorNode(GTE,$1,$3);
			$$->TYPE = TYPE_bool;}
			else {printf("\nTYPE ERROR GREATER THAN OR EQUAL"); exit(0);}}
			
	| expr AND expr  {if(($1->TYPE != TYPE_bool) || ($3->TYPE != TYPE_bool)) { printf("\nTYPE ERROR AND"); exit(0);}
			$$ = makeOperatorNode(AND,$1,$3);
			$$->TYPE = TYPE_bool;}
	| expr OR expr  {if(($1->TYPE != TYPE_bool) || ($3->TYPE != TYPE_bool)) { printf("\nTYPE ERROR OR"); exit(0);}
			$$ = makeOperatorNode(OR,$1,$3);
			$$->TYPE = TYPE_bool;}
	| NOT expr  {if($2->TYPE != TYPE_bool) { printf("\nTYPE ERROR NOT"); exit(0);}
			$$ = makeOperatorNode(NOT,$2,NULL);
			$$->TYPE = TYPE_bool;}
	| '(' expr ')' {$$ = $2;}
	| NUM {$$ = $1;
		$$->TYPE = TYPE_int;}
	| ID { if(Lsearch($1->NAME)==NULL)
     		{
     			printf("\nVariable undeclared");
     			exit(0);
     		}
	
		$$->TYPE = Lsearch($1->NAME)->type;

		//printf("%d expr",$1->TYPE);
                $$ = $1;}
 	| ID '[' expr ']' {if(Lsearch($1->NAME)==NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(0);
		     		}
			 	$$ = TreeCreate(TYPE_void,ARRAY,0,$1->NAME,$1,$3);
				$$->TYPE = Lsearch($1->NAME)->type;}
	|True {$$=$1;
		$$->TYPE = TYPE_bool;}
	|False {$$=$1;
		$$->TYPE = TYPE_bool;}
	;

stmt : ID ASGN expr ';' {//printf("%d %d",Lsearch($1->NAME)->type,$3->TYPE); 
			if((((Lsearch($1->NAME)->type) == TYPE_int) && ($3->TYPE == TYPE_int)) ||  (((Lsearch($1->NAME)->type) == TYPE_int) && ($3->TYPE == TYPE_intarray)))
			{
				if(Lsearch($1->NAME)==NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(0);
		     		}
				$$=TreeCreate(TYPE_void,ASGN,0,NULL,$1,$3);
			}
			else
			{printf("TYPE ERROR ASGN"); exit(0);}}
	| READ '(' ID ')'';' {if((Lsearch($3->NAME)->type) != TYPE_int) {printf("TYPE ERROR READ"); exit(0);}
				if(Lsearch($3->NAME)==NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(0);
		     		}
				$$=TreeCreate(TYPE_void,READ,0,NULL,$3,NULL);}
	| WRITE '('expr ')' ';' {if(($3->TYPE == TYPE_int) || ($3->TYPE == TYPE_intarray)) {$$=TreeCreate(TYPE_void,WRITE,0,NULL,$3,NULL);}
				else {printf("TYPE ERROR WRITE"); exit(0);}}
				
	| IF '(' expr ')' THEN slist ENDIF ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR IF"); exit(0);}
						 $$=TreeCreate(TYPE_void,IF,0,NULL,$3,$6);}
	| WHILE '(' expr ')' DO slist ENDWHILE  ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR WHILE"); exit(0);}
						$$=TreeCreate(TYPE_void,WHILE,0,NULL,$3,$6);}	
	| ID '[' expr ']' ASGN expr ';' {temp = Lsearch($1->NAME);

					if(temp == NULL)
				     		{
				     			printf("\nVariable undeclared");
				     			exit(0);
				     		}

					if((((temp->type) == TYPE_intarray) && ($6->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) ||  (((temp->type) == TYPE_intarray) && ($6->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)))
						{$$ = TreeCreate(TYPE_void,ARRAYASGN,0,$1->NAME,$3,$6);}
					else
					{printf("TYPE ERROR ASGN"); exit(0);}}

	| READ '(' ID '[' ID ']' ')' ';' {if((Lsearch($3->NAME)->type) == TYPE_intarray) 
						{if(Lsearch($3->NAME)==NULL)
				     		{
				     			printf("\nVariable undeclared");
				     			exit(0);
				     		}
						$$ = TreeCreate(TYPE_void,READARRAY,0,NULL,$3,$5);}
						else {printf("TYPE ERROR READ"); exit(0);}}
						
	| READ '(' ID '[' NUM ']' ')' ';' {if((Lsearch($3->NAME)->type) == TYPE_intarray) 
						{if(Lsearch($3->NAME)==NULL)
				     		{
				     			printf("\nVariable undeclared");
				     			exit(0);
				     		}
						$$ = TreeCreate(TYPE_void,READARRAY,0,NULL,$3,$5);}
						else {printf("TYPE ERROR READ"); exit(0);}}
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s", s);
}

int main(int argc,char* argv[])
{ 
	fp=fopen("in6.txt","r");
	if(fp)
		yyin=fp;

    	yyparse(); 
    	return 1; 
}
