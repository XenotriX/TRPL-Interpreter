%{
#include "parser.h"
%}

digit  [0-9]
letter [a-zA-Z]

%%
"+"                           return PLUS;
"-"                           return MINUS;
"*"                           return TIMES;
"/"                           return DIVIDED;
"("                           return LPARENT;
")"                           return RPARENT;
"{"                           return LBRACE;
"}"                           return RBRACE;
"["                           return LBRACKET;
"]"                           return RBRACKET;
","                           return COMMA;
"."                           return PERIOD;
":="                          return BECOMES;
"="                           return ASSIG;
"=="                          return EQUAL;
"<"                           return LESS;
">"                           return GREATER;
"<="                          return LESS_EQL;
">="                          return GREATER_EQL;
":"                           return COLON;
"var"                         return VARDEC;
"const"                       return CONSTDEC;
"print"                       return PRINT;
"load"                        return LOAD;
"typeOf"                      return TYPEOF;
"exit"                        return EXIT;
{letter}({letter}|{digit})*   return IDENTIFIER;
{digit}*("."{digit}*)?        return NUMBER;
\".*\"                        return STRING;
[\n\r]+                       return EOL;
[ \t]                         /* Ignore whitespaces */
.                             return UNKNOWN;
%%

int yywrap(void)
{
  return 1;
}
