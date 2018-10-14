%{	
  	#define YYSTYPE struct tnode*
  	#include<stdio.h>
  	#include<stdlib.h>
	#include<string.h>
  	#include "y.tab.h"
  	#include "stage5.c"
%}

%%

"read"  {return READ;}
"write" {return WRITE;}
"if"    {return IF;}
"then"  {return THEN;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"do"    {return DO;}
"endwhile" {return ENDWHILE;}
"begin" {return BEGINPRO;}
"end" {return  END;}
"int" {return INT;}
"decl" {return DECL;}
"enddecl" {return ENDDECL;}
[a-z]+  { char*varn = malloc(sizeof(char));
		 strcpy(varn,yytext);
		 yylval=TreeCreate(-5,ID,0,varn,NULL,NULL);
		 return ID;  }
[0-9]+ {yylval=makeLeafNode(atoi(yytext));
		return NUM;}
[=]    {return ASGN;}
[+]    {return PLUS;}
[*]    {return MUL; }
[>]    {return GT;}
[<]    {return LT;}
"=="  {return EQ;}
[ \t]	{}
[\n]   {}
.      {return *yytext;}
%%
int yywrap()
{
	return 1;
}
