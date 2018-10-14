%{
    #include <stdio.h> 
	  #include "y.tab.h"
%}

%%

[i][f] { return IF; }
"("  { return OPEN;}
")"  {return CLOSE;}
[sS][0-9]* {return S;}
“<”|”>”|”==”|”!=”|”<=”|”>=” { return RELOP; }
[0-9]+ { return NUMBER; }
[a-zA-Z][a-zA-Z0-9_]* { return ID; }
\n { ; }
. { return yytext[0]; }

%%

yywrap(){
	return 1;
}
