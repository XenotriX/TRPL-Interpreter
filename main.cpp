#include <iostream>
#include <vector>
#include "parser/parser.hpp"
#include "repl/repl.hpp"
#include "interpreter/interpreter.hpp"

int main (int argc, char *argv[])
{
  Interpreter interpreter;
  REPL repl;
  Parser parser;

  interpreter.addEventListener([&repl](LogLevel level, std::string input){repl.print(level, input);});
  repl.addEventListener([&interpreter, &parser](std::string input) {
    std::vector<ast::Statement*> stmts = parser.parse(input);
    interpreter.exec(stmts);
  });

  repl.start();
}

