%{		
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "stage9.h"
	#include "stage9.c"

	//FILE *ft;
	//extern FILE *yyin;
	struct tnode* t;
	int yylex(void);
	int yyerror(char const *);
	int gtype,ftype,ltype;
	struct Gsymbol *gtemp;
	struct Lsymbol *ltemp;
	struct Paramstruct *p,*param_list;
%}

%union
{
	struct tnode *nptr;
	struct Paramstruct *arg;
};

%token IDENTIFIER READ WRITE NUM NL PLUS MUL ASGN LT GT EQ IF ELSE THEN WHILE DO ENDWHILE ENDIF STMT END BEGINPRO INT ARRAY ARRAYASGN READARRAY DECL ENDDECL MINUS DIV MOD LTE GTE NEQ AND NOT OR STR True False MAIN RETURN
%right ASGN
%left AND OR
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%type <nptr> NUM  
%type <nptr> IDENTIFIER
%type <nptr> expr
%type <nptr> slist
%type <nptr> stmt
%type <arg> Paramlist
%type <arg> flist
%type <arg> fvar
%type <nptr> Body
%type <nptr> retstmt
%type <nptr> arglist
%type <nptr> Fdef
%type <arg> fid
%type <nptr> Mainblock

%%

S : GDefBlock Fdeflist Mainblock {fclose(fp); fp=fopen("inter.xsm","r+"); f=fopen("final.xsm","w+"); finalgen(fp,f); fclose(f); exit(1);}
	;

GDefBlock : DECL GDeclList ENDDECL {}
	;

GDeclList : GDeclList GDecl {}
	 | GDecl {}
	;

GDecl : type_gid gidlist ';'{}
	;

type_gid : INT {gtype = 0;}
	| STR {gtype = 1;}
	;

gidlist : gidlist ',' gid {}
	| gid {}
	;

gid : IDENTIFIER { if(gtype == 0) {Ginstall($1->NAME,TYPE_int,1,NULL);}
			else if(gtype == 1) {Ginstall($1->NAME,TYPE_str,1,NULL);}}

	| IDENTIFIER '[' NUM ']' { if(gtype == 0) {Ginstall($1->NAME,TYPE_intarray,$3->VALUE,NULL);}
					if(gtype == 1) {Ginstall($1->NAME,TYPE_strarray,$3->VALUE,NULL);}}

	| IDENTIFIER '(' Paramlist ')' { if(gtype == 0) {Ginstall($1->NAME,TYPE_func_int,1,$3);}
						if(gtype == 1) {Ginstall($1->NAME,TYPE_func_str,1,$3);}
						reinitialise_lvar();}	

	| IDENTIFIER '(' ')' { if(gtype == 0) {Ginstall($1->NAME,TYPE_func_int,1,NULL);}
						if(gtype == 1) {Ginstall($1->NAME,TYPE_func_str,1,NULL);}
						reinitialise_lvar();}
	;	

Paramlist : Paramlist flist { p = $1;
				while(p->next != NULL)
				{ p = p->next;}
				p->next = $2;
				$$ = $1; }
	| flist {$$ = $1;}
	;

flist : fvar_type fvar ';' {$$ = $2;}
	;

fvar_type : INT {ftype = 0;}
	| STR {ftype = 1;}
	;

fvar : fvar ',' fid { p = $1;
				while(p->next != NULL)
				{ p = p->next;}
				p->next = $3;
				$$ = $1; }
	| fid {$$ = $1;}
	;

fid : IDENTIFIER {if(ftype == 0) {$$ = install_list(TYPE_int,$1->NAME,TYPE_param_val);}
			else if(ftype == 1) {$$ = install_list(TYPE_str,$1->NAME,TYPE_param_val);}}

	| '&' IDENTIFIER {if(ftype == 0) {$$ = install_list(TYPE_int,$2->NAME,TYPE_param_ref);}
			else if(ftype == 1) {$$ = install_list(TYPE_str,$2->NAME,TYPE_param_ref);}}
	;
	
Fdeflist : Fdeflist Fdef {}
	| Fdef {}
	;	

Fdef : type_gid IDENTIFIER '(' Paramlist ')' '{' LDefblock Body '}' {	gtemp = Gsearch($2->NAME);

									if(gtemp == NULL)
									{
										printf("Error : Undefined Function %s\n",$2->NAME);
										exit(1);
									}

									if(!(gtemp->type == TYPE_func_int && gtype == 0) && !(gtemp->type == TYPE_func_str && gtype == 1))
									{
										printf("Error : Type mismatch of the function\n");
										exit(1);
									}

									if(!(gtemp->type == TYPE_func_int && $8->ptr2->TYPE == TYPE_int) && !(gtemp->type == TYPE_func_str && $8->ptr2->TYPE == TYPE_str))
									{
										printf("Error : Conflicting type of the function and return type of the function\n");
										exit(1);	
									}

									p = gtemp->paramlist;
									param_list = $4;

									while(p != NULL && param_list != NULL)
									{
										if(strcmp(p->pname,param_list->pname) != 0)
										{
											printf("Error : Name mismatch of the argument\n");
											exit(1);
										}

										else if(p->ptype != param_list->ptype)
										{
											printf("Error : Type mismatch of the argument\n");
											exit(1);
										}

										else if(p->pass_type != param_list->pass_type)
										{
											printf("Error :Pass Type mismatch of the argument\n");
											exit(1);
										}

										p = p->next;
										param_list = param_list->next;
									}

								   	if(p != NULL || param_list != NULL)
									{	
										printf("Error : Mismatch in number of arguments\n");
										exit(1);
									}

	 								$2->Lentry = Lhead;
									codegen_func($2->NAME,$8);
									reinitialise_lvar(); }

	| type_gid IDENTIFIER '(' ')' '{' LDefblock Body '}' { gtemp = Gsearch($2->NAME);

									if(gtemp == NULL)
									{
										printf("Error : Undefined Function %s\n",$2->NAME);
										exit(1);
									}

									if(!(gtemp->type == TYPE_func_int && gtype == 0) && !(gtemp->type == TYPE_func_str && gtype == 1))
									{
										printf("Error : Type mismatch of the function\n");
										exit(1);
									}

									if(!(gtemp->type == TYPE_func_int && $7->ptr2->TYPE == TYPE_int) && !(gtemp->type == TYPE_func_str && $7->ptr2->TYPE == TYPE_str))
									{
										printf("Error : Conflicting type of the function and return type of the function\n");
										exit(1);	
									}

									$2->Lentry = Lhead;
									codegen_func($2->NAME,$7);
									reinitialise_lvar(); }
	;

LDefblock : DECL list1 ENDDECL {}
	;

list1 : list1 Llist {}
	| Llist {}
	; 

Llist : lvar_type lvar ';' {}
	;

lvar_type : INT {ltype = 0;}
	| STR {ltype = 0;}
	;

lvar : lvar ',' IDENTIFIER {if(ltype == 0) {Linstall($3->NAME,TYPE_int);}
				else if(ltype == 1) {Linstall($3->NAME,TYPE_str);}}

	| IDENTIFIER {if(ltype == 0) {Linstall($1->NAME,TYPE_int);}
				else if(ltype == 1) {Linstall($1->NAME,TYPE_str);}}
	;

Mainblock : INT MAIN '(' ')' '{' LDefblock BEGINPRO slist END '}' { Ginstall("MAIN",TYPE_func_int,1,NULL);
								generate($8);}
	;
				
Body : BEGINPRO slist retstmt END { $$ = TreeCreate(TYPE_int,NODE_STMT,0,NULL,$2,$3,NULL,NULL);}
	;

retstmt : RETURN expr ';' {$$ = TreeCreate(TYPE_int,NODE_RET,0,NULL,$2,NULL,NULL,NULL); }
	;
												
slist : slist stmt {$$ = TreeCreate(TYPE_void,NODE_STMT,0,NULL,$1,$2,NULL,NULL);}
	  | stmt  {$$ = $1;}
	  ;

expr : expr PLUS expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR PLUS"); exit(1);}
			else {$$ = makeOperatorNode(NODE_PLUS,$1,$3);
			$$->TYPE = TYPE_int;} }

	| expr MUL expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR MUL"); exit(1);}
			else {$$ = makeOperatorNode(NODE_MUL,$1,$3);
			$$->TYPE = TYPE_int;} }

	| expr MINUS expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR MINUS"); exit(1);}
			else {$$ = makeOperatorNode(NODE_MINUS,$1,$3);
			$$->TYPE = TYPE_int;} }

	| expr DIV expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR DIV"); exit(1);}
			else {$$ = makeOperatorNode(NODE_DIV,$1,$3);
			$$->TYPE = TYPE_int;} }

	| expr MOD expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR MOD"); exit(1);}
			else {$$ = makeOperatorNode(NODE_MOD,$1,$3);
			$$->TYPE = TYPE_int;} }
			
	| expr LT expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR LESSER THAN"); exit(1);}
			else {$$ = makeOperatorNode(NODE_LT,$1,$3);
			$$->TYPE = TYPE_bool;} }
			
	| expr GT expr {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR GREATER THAN"); exit(1);}
			else {$$ = makeOperatorNode(NODE_GT,$1,$3);
			$$->TYPE = TYPE_bool;} }

	| expr EQ expr  {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR EQUAL"); exit(1);}
			else {$$ = makeOperatorNode(NODE_EQ,$1,$3);
			$$->TYPE = TYPE_bool;} }

	| expr NEQ expr  {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR NOT EQUAL"); exit(1);}
			else {$$ = makeOperatorNode(NODE_NEQ,$1,$3);
			$$->TYPE = TYPE_bool;} }
			
	| expr LTE expr  {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR LESS THAN OR EQUAL"); exit(1);}
			else {$$ = makeOperatorNode(NODE_LTE,$1,$3);
			$$->TYPE = TYPE_bool;} }

	| expr GTE expr  {if($1->TYPE == TYPE_str || $3->TYPE == TYPE_str || $1->TYPE ==TYPE_bool || $3->TYPE == TYPE_bool)
				{printf("\nTYPE ERROR GREATER THAN OR EQUAL"); exit(1);}
			else {$$ = makeOperatorNode(NODE_GTE,$1,$3);
			$$->TYPE = TYPE_bool;} }
			
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

	| IDENTIFIER {ltemp = Lsearch($1->NAME);
			if(ltemp == NULL)
			{
				gtemp = Gsearch($1->NAME);
				if(gtemp == NULL)
				{
					printf("Error : Variable undeclared\n");
					exit(1);
				}
				else if(gtemp->type == TYPE_intarray || gtemp->type == TYPE_strarray)
				{
					printf("Error: Invalid access to array\n");
					exit(1);
				}
				else
				{
					$$->TYPE = gtemp->type;
					$1->Gentry = gtemp;
                			$$ = $1;
				}
			}
			else
			{
				if(ltemp->type == TYPE_int || ltemp->type == TYPE_arg_int || ltemp->type == TYPE_arg_ref_int)
					$$->TYPE = TYPE_int;
				else
					$$->TYPE = TYPE_str;
				$1->Lentry = ltemp;
                		$$ = $1;
			} }
		

 	| IDENTIFIER '[' expr ']' {gtemp = Gsearch($1->NAME);
				if(gtemp == NULL)
		     		{
		     			printf("\nVariable undeclared");
		     			exit(1);
		     		}
				else if(gtemp->type ==TYPE_int || gtemp->type == TYPE_str)
				{
					printf("Error : Invalid access to variables\n");
					exit(1);
				}
			 	$$ = TreeCreate(TYPE_void,NODE_ARRAY,0,$1->NAME,$1,$3,NULL,NULL);
				if(gtemp->type == TYPE_intarray)
					$$->TYPE = TYPE_int;
				else
					$$->TYPE = TYPE_str;
				$1->Gentry = gtemp;}

	| IDENTIFIER '(' arglist ')'	{ gtemp = Gsearch($1->NAME);
					if(gtemp == NULL)
					{
						printf("ERROR: undefined function\n");
						exit(1);
					}
					else
					{
						$1->Gentry = gtemp;
						if(gtemp->type == TYPE_func_int)
							$$->TYPE = TYPE_int;
						else
							$$->TYPE = TYPE_str;
						$$ = TreeCreate($1->TYPE,NODE_FUNCTION,0,$1->NAME,NULL,NULL,NULL,$3);
					} } 

	| IDENTIFIER '(' ')'	{ gtemp = Gsearch($1->NAME);
					if(gtemp == NULL)
					{
						printf("ERROR: undefined function\n");
						exit(1);
					}
					
					else
					{
						$1->Gentry = gtemp;
						if(gtemp->type == TYPE_func_int)
							$$->TYPE = TYPE_int;
						else
							$$->TYPE = TYPE_str;
						$$ = TreeCreate($1->TYPE,NODE_FUNCTION,0,$1->NAME,NULL,NULL,NULL,NULL);
					} }                            
    	                                                  
    	                       
	;

arglist : arglist ',' expr {$$=TreeCreate(TYPE_void,NODE_PARAMETERS,0,NULL,$1,$3,NULL,NULL);}
	| expr {$$=TreeCreate(TYPE_void,NODE_PARAMETERS,0,NULL,NULL,$1,NULL,NULL);}
	;

stmt : IDENTIFIER ASGN expr ';' {ltemp = Lsearch($1->NAME);
			if(ltemp == NULL)
			{
				gtemp = Gsearch($1->NAME);
				if(gtemp == NULL)
				{
					printf("Error : Variable undeclared\n");
					exit(1);
				}
				else if(gtemp->type == TYPE_intarray || gtemp->type == TYPE_strarray)
				{
					printf("Error: Invalid access to array\n");
					exit(1);
				}
				else
				{
					$1->TYPE = gtemp->type;
					

					if($1->TYPE != $3->TYPE)
						{printf("TYPE ERROR ASGN"); exit(1);}

					else
						$1->Gentry = gtemp;
				}
			}
			else
			{
				$1->TYPE = ltemp->type;

				if($1->TYPE != $3->TYPE)
					{printf("TYPE ERROR ASGN"); exit(1);}

				else
					$1->Lentry = ltemp;
			}
			$$=TreeCreate(TYPE_void,NODE_ASGN,0,NULL,$1,$3,NULL,NULL);}

	| READ '(' IDENTIFIER ')'';' {ltemp = Lsearch($3->NAME);
					if(ltemp == NULL)
					{
						gtemp = Gsearch($3->NAME);
						if(gtemp == NULL)
						{
							printf("Error : Variable undeclared\n");
							exit(1);
						}
						else if(gtemp->type == TYPE_intarray || gtemp->type == TYPE_strarray)
						{
							printf("Error: Invalid access to array\n");
							exit(1);
						}
						else
						{
							$3->Gentry = gtemp;
							if((gtemp->type) != TYPE_int) 
								{printf("TYPE ERROR READ"); exit(1);}
							else
								{$$=TreeCreate(TYPE_void,NODE_READ,0,NULL,$3,NULL,NULL,NULL);}
						}
					}
					else
					{
						$3->Lentry = ltemp;
						if((ltemp->type) != TYPE_int) 
							{printf("TYPE ERROR READ"); exit(1);}
						else
							{$$=TreeCreate(TYPE_void,NODE_READ,0,NULL,$3,NULL,NULL,NULL);}
					} }

	| WRITE '('expr ')' ';' {if(($3->TYPE == TYPE_int) || ($3->TYPE == TYPE_intarray) || ($3->TYPE == TYPE_func_int)) {$$=TreeCreate(TYPE_void,NODE_WRITE,0,NULL,$3,NULL,NULL,NULL);}
				else {printf("TYPE ERROR WRITE"); exit(0);}}
				
	| IF '(' expr ')' THEN slist ENDIF ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR IF"); exit(0);}
						 $$=TreeCreate(TYPE_void,NODE_IF,0,NULL,$3,$6,NULL,NULL);}

	| IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void) || ($8->TYPE != TYPE_void)) {printf("TYPE ERROR IF"); exit(0);}
						 $$=TreeCreate(TYPE_void,NODE_ELSE,0,NULL,$3,$6,$8,NULL);}

	| WHILE '(' expr ')' DO slist ENDWHILE  ';' {if(($3->TYPE != TYPE_bool) || ($6->TYPE != TYPE_void)) {printf("TYPE ERROR WHILE"); exit(0);}
						$$=TreeCreate(TYPE_void,NODE_WHILE,0,NULL,$3,$6,NULL,NULL);}
	
	| IDENTIFIER '[' expr ']' ASGN expr ';' {gtemp = Gsearch($1->NAME);
							if(gtemp == NULL)
					     		{
					     			printf("\nVariable undeclared");
					     			exit(1);
					     		}
							else if(gtemp->type ==TYPE_int || gtemp->type == TYPE_str)
							{
								printf("Error : Invalid access to variables\n");
								exit(1);
							}
							$1->Gentry = gtemp;

							if((((gtemp->type) == TYPE_intarray) && ($6->TYPE == TYPE_int) && ($3->TYPE == TYPE_int)) ||  (((gtemp->type) == TYPE_intarray) && ($6->TYPE == TYPE_intarray) && ($3->TYPE == TYPE_intarray)))
								{$$ = TreeCreate(TYPE_void,NODE_ARRAYASGN,0,NULL,$1,$3,$6,NULL);}

							else if((((gtemp->type) == TYPE_strarray) && ($3->TYPE == TYPE_str)) || (((gtemp->type) == TYPE_strarray) && ($3->TYPE == TYPE_strarray)))
								{$$ = TreeCreate(TYPE_void,NODE_ARRAYASGN,0,NULL,$1,$3,$6,NULL);}

							else
								{printf("TYPE ERROR ASGN"); exit(1);} }

	| READ '(' IDENTIFIER '[' expr ']' ')' ';' {gtemp = Gsearch($3->NAME);
							if(gtemp == NULL)
					     		{
					     			printf("\nVariable undeclared");
					     			exit(1);
					     		}
							else if(gtemp->type ==TYPE_int || gtemp->type == TYPE_str)
							{
								printf("Error : Invalid access to variables\n");
								exit(1);
							}
							$3->Gentry = gtemp;
						
							if(((gtemp->type) == TYPE_intarray) && ($5->TYPE == TYPE_int)){ 
							$$ = TreeCreate(TYPE_void,NODE_READARRAY,0,NULL,$3,$5,NULL,NULL);}

							else {printf("TYPE ERROR READ"); exit(1);}}
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s", s);
}
int main()
{ 
	/*ft = fopen("quicksort.txt","r");
	if(ft)
		yyin = ft;*/
	fp=fopen("inter.xsm","w+");
	
	//printf("HEADER\n");
	fprintf(fp,"0\n"); //XMAGIC
	fprintf(fp,"F0\n"); //ENTRY POINT
	fprintf(fp,"0\n"); //TEXT SIZE
	fprintf(fp,"0\n"); //DATA SIZE
	fprintf(fp,"0\n"); //HEAP SIZE
	fprintf(fp,"0\n"); //STACK SIZE
	fprintf(fp,"1\n"); //LIBRARY FLAG
	fprintf(fp,"0\n"); //UNUSED
	//printf("ENDHEADER\n"); 
	
	yyparse();
	fclose(fp);
	return 1;
}
