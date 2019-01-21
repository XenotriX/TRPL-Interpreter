#include <iostream>
#include <vector>
#include "parser.hxx"
#include "repl/repl.hpp"
#include "interpreter/interpreter.hpp"

struct myParserOutput : ParserOutput
{
  std::vector<ast::Statement*> stmts;

  void addStatement(ast::Statement* stmt) override
  {
    stmts.push_back(stmt);
  }

  std::vector<ast::Statement*> getStatements()
  {
    return stmts;
  }
};

int main (int argc, char *argv[])
{
  Interpreter interpreter;
  REPL repl;

  interpreter.addEventListener([&repl](LogLevel level, std::string input){repl.print(level, input);});
  repl.addEventListener([&interpreter](std::string input) {
    myParserOutput cb;
    std::vector<std::string> lines;
    lines.push_back(input);
    parse(lines, &cb);
    auto stmts = cb.getStatements();
    interpreter.exec(stmts);
  });

  repl.start();
}

