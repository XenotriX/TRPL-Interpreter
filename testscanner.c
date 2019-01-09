#include <stdio.h>
#include "tokens.h"

extern int yylex();
extern char* yytext;

char* tokens[] = {
  "UNKNOWN",
  "PLUS",
  "MINUS",
  "TIMES",
  "DIVIDED",
  "LPARENT",
  "RPARENT",
  "COMMA",
  "PERIOD",
  "BECOMES",
  "ASSIG",
  "EQUAL",
  "LESS",
  "GREATER",
  "LESS_EQL",
  "GREATER_EQL",
  "IDENTIFIER",
  "NUMBER",
  "STRING",
  "VARDEC",
  "CONSTDEC",
  "EOL",
  "PRINT",
  "LOAD",
  "TYPEOF",
  "EXIT",
  "COLON"
};

int main(void)
{
  int ntoken, vtoken;

  ntoken = yylex();
  while(ntoken) {
    printf("%s \"%s\"\n", tokens[ntoken], (ntoken != EOL) ? yytext : "");
    ntoken = yylex();
  }
  return 0;
}
