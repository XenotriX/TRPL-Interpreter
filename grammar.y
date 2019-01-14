%{
void yyerror(const char* s);
extern int yylex();
#include <stdio.h>
#define YYDEBUG 1
%}

%define parse.error verbose
%start program
%token ASSIG
%token BECOMES
%token IDENTIFIER
%token NUMBER
%token PLUS
%token MINUS
%token TIMES
%token DIVIDED
%token LESS
%token GREATER
%token LESS_EQL
%token GREATER_EQL
%token STRING
%token RPARENT
%token LPARENT
%token RBRACE
%token LBRACE
%token RBRACKET
%token LBRACKET
%token COLON
%token VARDEC
%token COMMA
%token PRINT
%token LOAD
%token TYPEOF
%token EXIT
%token EOL
%token PERIOD
%token CONSTDEC
%token EQUAL
%token UNKNOWN

%%
program    : statement
           | statement program
           ;

expression : IDENTIFIER
           | pattern
           | literal
           | expression operator expression
           ;

pattern    : IDENTIFIER PERIOD IDENTIFIER
           | IDENTIFIER PERIOD pattern
           ;

literal    : NUMBER
           | STRING
           | array
           | object
           ;

property   : IDENTIFIER COLON expression
           ;

properties : property
           | property COMMA properties
           ;

array      : LBRACKET list RBRACKET
           ;

object     : LBRACE properties RBRACE
           ;

list       : expression
           | expression COMMA list
           ;

operator   : PLUS
           | MINUS
           | TIMES
           | DIVIDED
           | EQUAL
           | LESS
           | GREATER
           | LESS_EQL
           | GREATER_EQL
           ;

assig_op   : ASSIG
           | BECOMES
           ;

assignment : IDENTIFIER assig_op expression
           ;

vardec     : VARDEC IDENTIFIER
           | VARDEC IDENTIFIER assig_op expression
           ;

constdec   : CONSTDEC IDENTIFIER assig_op expression
           ;

statement  : vardec
           | expression EOL
           | assignment EOL
           | vardec EOL
           | constdec EOL
           | command EOL
           ;

command    : print
           | exit
           | load
           | typeof
           ;

print      : PRINT list
           ;

exit       : EXIT
           ;

load       : LOAD STRING
           ;

typeof     : TYPEOF expression
           ;

%%

void yyerror(const char* s)
{
  printf("%s", s);
}

