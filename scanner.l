%option c++
%option 8bit warn nodefault
%option noyywrap

digit  [0-9]
letter [a-zA-Z]

%{
    #include <stdexcept>
    #include <cstdlib>
    #include "parser.hxx"
    #include "scanner.hpp"

    // utility macros to simplify the actions
    #define YIELD_TOKEN(tok, val, type)                 \
                    yylval->build<type>(val);           \
                    return tok;

    #define YY_TXT                  std::string(yytext, yyleng)
    #define YY_NUM                  std::atof(yytext)

    #define INT_TOKEN(tok, val)     YIELD_TOKEN(tok, val, int)
    #define NUM_TOKEN(tok)          YIELD_TOKEN(tok, YY_NUM, double)
    #define STR_TOKEN(tok)          YIELD_TOKEN(tok, YY_TXT, std::string)
    #define LITERAL                 return yytext[0];

    // before executing an action, set the length of the location from
    // the length of the matched pattern:
    #define YY_USER_ACTION          yylloc->columns(yyleng);
%}

%%

%{
  yylloc->step();
%}

"var"                         return yy::parser::token::VARDEC;
{letter}({letter}|{digit})*   STR_TOKEN(yy::parser::token::IDENTIFIER);
\n+                           return yy::parser::token::EOL;
[ \t]                         /* */
.                             return yy::parser::token::UNKNOWN;
%%

yy::scanner::scanner(std::istream* in, std::ostream* out)
  : yyFlexLexer(in, out)
{
}

int yyFlexLexer::yylex()
{
  throw std::logic_error("The yylex() exists for technical reasons and must not be used.");
}

