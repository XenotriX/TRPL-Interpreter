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

  void parse(const std::vector<std::string>&, ParserOutput*);
} 

%code {
    #include <iostream>     // cerr, endl
    #include <utility>      // move
    #include <string>
    #include <sstream>

    #include "scanner.hpp"

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
}

%start program
%token <std::string> IDENTIFIER
%token VARDEC
%token UNKNOWN
%token EOL
%type <ast::VarDeclaration*> vardec
%type <ast::Statement*> statement

%%
program    : statement          { cb->addStatement($1); }
           | statement program
           ;

vardec     : VARDEC IDENTIFIER { $$ = new ast::VarDeclaration($2); }
           ;

statement  : vardec             { $$ = static_cast<ast::Statement*>($1); }
           ;

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
  throw yy::parser::syntax_error(l, m);
}

void parse(const std::vector<std::string>& exprs, ParserOutput* out)
{
  for (int row = 0; row < exprs.size(); ++row) {
    const std::string& line = exprs[row];
    std::istringstream in(line);
    yy::scanner scanner(&in);
    yy::parser parser(&scanner, out);
    try {
      int result = parser.parse();
      if (result != 0) {
        // Not sure if this can even happen
        throw std::runtime_error("Unknown parsing error");
      }
    }
    catch (yy::parser::syntax_error& e) {
      // improve error messages by adding location information:
      int col = e.location.begin.column;
      int len = 1 + e.location.end.column - col;
      // TODO: The reported location is not entirely satisfying. Any
      // chances for improvement?
      std::ostringstream msg;
      msg << e.what() << "\n"
          << "in row " << row << " col " << col << ":\n\n"
          << "    " << line << "\n"
          << "    " << std::string(col-1, ' ') << std::string(len, '^');
      throw yy::parser::syntax_error(e.location, msg.str());
    }
  }
}

