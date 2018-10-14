%{
      #include <stdio.h>
      #include <stdlib.h>

%}

%token IF RELOP S NUMBER ID OPEN CLOSE

%%

start : if_stmt { printf(“No of nested if statements=%d\n”,count); exit(0);}
;
if_stmt : IF OPEN cond CLOSE if_stmt {count++;}
| S
;
cond : x RELOP x
;
x : ID
| NUMBER
;

%%

int yyerror(char *msg)
{
printf(“Invalid Expression\n”);
exit(0);
}
int main ()
{
printf(“Enter the statement”);
yyparse();
return(1);
}
