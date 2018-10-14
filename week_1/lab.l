%option noyywrap

%{
	#include<stdio.h>
	#include<stdlib.h>
	int sum = 0,num,rem,res;
%}

digit [0-9]+

%%
{digit} {num = atoi(yytext); while(num !=0) { rem = num%10; sum+=rem; num = num/10;} return sum;}
%%

int main()
{
	res = yylex();
	printf("\nthe sum is:  %d",res);

	return 1;
}
