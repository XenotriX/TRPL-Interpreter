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

"+"                           return yy::parser::token::PLUS;
"-"                           return yy::parser::token::MINUS;
"*"                           return yy::parser::token::TIMES;
"/"                           return yy::parser::token::DIVIDED;
"("                           return yy::parser::token::LPARENT;
")"                           return yy::parser::token::RPARENT;
"{"                           return yy::parser::token::LBRACE;
"}"                           return yy::parser::token::RBRACE;
"["                           return yy::parser::token::LBRACKET;
"]"                           return yy::parser::token::RBRACKET;
","                           return yy::parser::token::COMMA;
"."                           return yy::parser::token::PERIOD;
":="                          return yy::parser::token::BECOMES;
"="                           return yy::parser::token::ASSIG;
"=="                          return yy::parser::token::EQUAL;
"<"                           return yy::parser::token::LESS;
">"                           return yy::parser::token::GREATER;
"<="                          return yy::parser::token::LESS_EQL;
">="                          return yy::parser::token::GREATER_EQL;
":"                           return yy::parser::token::COLON;
"var"                         return yy::parser::token::VARDEC;
"const"                       return yy::parser::token::CONSTDEC;
"print"                       return yy::parser::token::PRINT;
"load"                        return yy::parser::token::LOAD;
"typeOf"                      return yy::parser::token::TYPEOF;
"exit"                        return yy::parser::token::EXIT;
{letter}({letter}|{digit})*   STR_TOKEN(yy::parser::token::IDENTIFIER);
{digit}*("."{digit}*)?        NUM_TOKEN(yy::parser::token::NUMBER);
\".*\"                        return yy::parser::token::STRING;
[ \n\t]                       /* Ignore whitespaces */
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

