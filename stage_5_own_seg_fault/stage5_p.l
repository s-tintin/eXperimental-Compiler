%{
	#include<stdio.h>
	#include"y.tab.h"
	#include<stdlib.h>
	#include<string.h>
%}

%%
"decl" {return DECL;}
"enddecl" {return ENDDECL;}
"int" {return INT;}
"start" {return START;}
"end"   {return END;}
"read"  {return READ;}
"write" {return WRITE;}
"if"    {return IF;}
"then"  {return THEN;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"do"    {return DO;}
"endwhile" {return ENDWHILE;}
[0-9]+ {printf("\nLEX NUMBER"); 
	int num;
	num = atoi(yytext); 
	printf("%d",num);
	yylval = TreeCreate(0,0,num,NULL,NULL,NULL);
	return NUM;}
[a-z]+ {char *varname = malloc(sizeof(char)*50);
	strcpy(varname,yytext);
	yylval = TreeCreate(0,9,0,varname,NULL,NULL);
	
	return ID;}
[+] {return PLUS;}
[*] {return MUL;}
[=] {printf("\nlexassignment"); return ASSGN;}
"==" {return EQUAL;}
[>] {return GREATER;}
[<] {return LESSER;}
[ \t] {}
[\n] {}
. {return *yytext;}

%%

int yywrap(void)
{
	return 1;
}


