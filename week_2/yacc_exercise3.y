%{
      #include <stdio.h>
      #include <stdlib.h>
      typedef char YYSTYPE;
%}

%token CHARACTER NEWLINE
 
%%

start : expr NEWLINE  { 
                        printf("\nComplete\n");
                        exit(1);
                      }
  ;

expr:  expr '+' expr        {printf("+ ");}
  | expr '-' expr     {printf("- ");}
  | '(' expr ')'
  | CHARACTER             {printf("%c ",$1);}
  ;

%%

void yyerror(char const *s) 
{ 
    printf("yyerror  %s\n",s); 
    return ;
} 
int main()
{
  yyparse();
  return 1;
}
