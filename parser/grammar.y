%skeleton "lalr1.cc"
%require "3.2"

%parse-param {yy::scanner* scanner} {ParserOutput* cb}

%locations

%define api.value.type variant

%define parse.trace
%define parse.error verbose

%code requires {
  #include <stdexcept>
  #include <string>
  #include <vector>

  #include "ast.hpp"
  #include "location.hh"

  namespace yy {
    class scanner;
  };

  struct ParserOutput {
    virtual void addStatement(ast::Statement*) = 0;
  };
} 

%code {
    #include <iostream>     // cerr, endl
    #include <utility>      // move
    #include <string>

    #include "parser/scanner.hpp"

    using std::move;

    #undef yylex
    #define yylex scanner->lex

    // utility function to append a list element to a std::vector
    template <class T, class V>
    T&& enlist(T& t, V& v)
    {
        t.push_back(move(v));
        return move(t);
    }

  struct ExpectMore {};
}

%start program
%token <std::string> IDENTIFIER
%token ASSIG
%token BECOMES
%token <double> NUMBER
%token PLUS
%token MINUS
%token TIMES
%token DIVIDED
%token LESS
%token GREATER
%token LESS_EQL
%token GREATER_EQL
%token <std::string> STRING
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
%token FILE_END
%token IF
%token TRUE
%token FALSE
%type <ast::Identifier*> identifier
%type <ast::VarDeclaration*> vardec
%type <ast::Assignment*> assignment
%type <ast::Statement*> statement
%type <ast::Statement*> command
%type <ast::PrintStatement*> print
%type <ast::Expression*> literal
%type <ast::Expression*> expression
%type <std::vector<ast::Expression*>> list
%type <ast::Property*> property
%type <std::vector<ast::Property*>> properties
%type <ast::ObjectLiteral*> object
%type <ast::Branch*> branch
%type <ast::BooleanLiteral*> boolean
%type <std::vector<ast::Statement*>> scope
%type <std::vector<ast::Statement*>> statements

%%
program    : statement          { cb->addStatement($1); }
           | statement program
           ;

identifier : IDENTIFIER { $$ = new ast::Identifier($1); }
           ;

expression : identifier { $$ = $1; }
           | pattern
           | literal { $$ = $1; }
           | expression expression PLUS { $$ = new ast::Addition($1, $2); }
           | expression expression MINUS { $$ = new ast::Substraction($1, $2); }
           | expression expression TIMES { $$ = new ast::Multiplication($1, $2); }
           | expression expression DIVIDED { $$ = new ast::Division($1, $2); }
           ;

pattern    : IDENTIFIER PERIOD IDENTIFIER
           | IDENTIFIER PERIOD pattern
           ;

literal    : NUMBER { $$ = new ast::NumberLiteral($1); }
           | STRING { $$ = new ast::StringLiteral($1); }
           | boolean { $$ = $1; }
           | array
           | object { $$ = $1; }
           ;

boolean    : TRUE  { $$ = new ast::BooleanLiteral(true); }
           | FALSE { $$ = new ast::BooleanLiteral(false); }
           ;

property   : identifier COLON expression { $$ = new ast::Property($1, $3); }
           ;

properties : property                   { $$ = std::vector<ast::Property*>(); enlist($$, $1); }
           | property COMMA properties  { $$ = enlist($3, $1); }
           ;

array      : LBRACKET list RBRACKET
           ;

object     : LBRACE properties RBRACE { $$ = new ast::ObjectLiteral($2); }
           ;

list       : expression             { $$ = std::vector<ast::Expression*>(); enlist($$, $1); }
           | expression COMMA list  { $$ = enlist($3, $1); }
           ;

assig_op   : ASSIG
           | BECOMES
           ;

branch     : IF LPARENT expression RPARENT statement { $$ = new ast::Branch($3, $5); }
           | IF LPARENT expression RPARENT scope     { $$ = new ast::Branch($3, $5); }
           | IF LPARENT expression RPARENT           { throw ExpectMore(); }
           ;

scope      : LBRACE statements RBRACE                { $$ = $2; }
           | LBRACE statements                       { throw ExpectMore(); }
           | LBRACE                                  { throw ExpectMore(); }
           ;

assignment : identifier assig_op expression          { $$ = new ast::Assignment($1, $3); }
           ;

vardec     : VARDEC identifier { $$ = new ast::VarDeclaration($2); }
           | VARDEC identifier assig_op expression { $$ = new ast::VarDeclaration($2, $4); }
           ;

constdec   : CONSTDEC identifier assig_op expression
           ;

statement  : vardec                  { $$ = $1; }
           | assignment              { $$ = $1; }
           | branch                  { $$ = $1; }
           | constdec
           | command
           ;

statements : statement            { $$ = std::vector<ast::Statement*>(); enlist($$, $1); }
           | statements statement { $$ = enlist($1, $2); }
           ;

command    : print             { $$ = static_cast<ast::Statement*>($1); }
           | exit
           | load
           | typeof
           ;

print      : PRINT list { $$ = new ast::PrintStatement($2); }
           ;

exit       : EXIT
           ;

load       : LOAD STRING
           ;

typeof     : TYPEOF expression
           ;

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
  throw yy::parser::syntax_error(l, m);
}

