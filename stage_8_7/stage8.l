%{	
  	#include<stdio.h>
  	#include<stdlib.h>
	#include<string.h>
  	#include "y.tab.h"
	#include "stage8.h"
%}

%%
"read" {return READ;}
"write" {return WRITE;}
"if" {return IF;}
"then" {return THEN;}
"else" {return ELSE;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"do" {return DO;}
"endwhile" {return ENDWHILE;}
"begin" {return BEGINPRO;}
"end" {return  END;}
"int" {return INT;}
"decl" {return DECL;}
"enddecl" {return ENDDECL;}
"bool" {return BOOL;}
"true" {yylval=TreeCreate(TYPE_bool,NODE_True,1,NULL,NULL,NULL,NULL);
	return True;}
"false" {yylval=TreeCreate(TYPE_bool,NODE_False,0,NULL,NULL,NULL,NULL);
	return False;}
[a-zA-Z_][a-zA-Z_0-9]* {char *varn = malloc(sizeof(char));
	strcpy(varn,yytext);
	yylval=TreeCreate(TYPE_void,NODE_IDENTIFIER,0,varn,NULL,NULL,NULL);
	return IDENTIFIER;}
[0-9]+ {yylval=makeLeafNode(atoi(yytext));
	return NUM;}
[=] {return ASGN;}
[+] {return PLUS;}
[*] {return MUL; }
[-] {return MINUS;}
[/] {return DIV;}
[%] {return MOD;}
[>] {return GT;}
[<] {return LT;}
"==" {return EQ;}
"!=" {return NEQ;}
">=" {return GTE;}
"<=" {return LTE;}
"&&" {return AND;}
"||" {return OR;}
[!] {return NOT;}
[ \t]	{}
[\n]   {}
.      {return *yytext;}
%%

int yywrap()
{
	return 1;
}
