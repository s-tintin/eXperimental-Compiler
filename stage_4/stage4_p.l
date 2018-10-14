%{
	#include <stdio.h>
	#include "y.tab.h"
	#include <stdlib.h>

	int num;
	char varname;


%}

%%

[a-z] {varname = *yytext;
	yylval = TreeCreate(0,9,0,varname,NULL,NULL);
	return ID;}

[0-9]+ {num = atoi(yytext);
	yylval = TreeCreate(0,0,num,'n',NULL,NULL);
	return NUM;}

[=] {return ASSGN;}
[+] {return PLUS;}
[*] {return MUL;}
"==" {return EQUAL;}
[>] {return GREATER;}
[<] {return LESSER;}
"read" {return READ;}
"write" {return WRITE;}
"if" {return IF;}
"then" {return THEN;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"do" {return DO;}
"endwhile" {return ENDWHILE;}
[ \t] {}
[\n] {return NEWLINE;}
. {return *yytext;}

%%

int yywrap(void)
{
	return 1;
}
