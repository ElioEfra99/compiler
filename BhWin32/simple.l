/***************************************************************************
Scanner for the Simple language
***************************************************************************/
%{
/*=========================================================================
C-libraries and Token definitions
=========================================================================*/
#include <string.h> /* for strdup */
/*#include <stdlib.h> */ /* for atoi */
#include "simple.tab.h" /* for token definitions and yylval */
%}
/*=========================================================================
TOKEN Definitions
=========================================================================*/
DIGIT [0-9]
ID [a-z][a-z0-9]*
STR \"(\\.|[^"\\])*\"
/*=========================================================================
REGULAR EXPRESSIONS defining the tokens for the Simple language
=========================================================================*/
%%
"-=>" { return(ASSGNOP); }
{DIGIT}+ { yylval.intval = atoi( yytext );return(NUMBER); }
hacer { return(DO); }
sino { return(ELSE); }
gameover { return(END); }
finif { return(FI); }
si { return(IF); }
start { return(IN); }
ent { return(INTEGER); }
variables { return(LET); }
lire { return(READ); }
salto { return(SKIP); }
entons { return(THEN); }
mant { return(WHILE); }
imprimo { return(WRITE); }
printx { return(PRINTX); }
{STR} { yylval.id = (char *) strdup(yytext);return(STRING); }
{ID} { yylval.id = (char *) strdup(yytext);return(IDENTIFIER); }
[ \t\n]+ /* eat up whitespace */{ }
. { return(yytext[0]); }
%%

int yywrap(void){ return 0;}
/************************** End Scanner File */
