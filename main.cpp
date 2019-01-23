#include <iostream>
#include <vector>
#include "parser/parser.hpp"
#include "repl/repl.hpp"
#include "interpreter/interpreter.hpp"

struct ExpectMore {};

int main (int argc, char *argv[])
{
  Interpreter interpreter;
  REPL repl;
  Parser parser;

  parser.addEventListener([&repl](int indent) {
    repl.setIndent(indent);
  });
  interpreter.addEventListener([&repl](LogLevel level, std::string input){repl.print(level, input);});
  repl.addEventListener([&interpreter, &parser, &repl](std::string input) {
    std::vector<ast::Statement*> stmts = parser.parse(input);
    interpreter.exec(stmts);
  });

  repl.start();
}

