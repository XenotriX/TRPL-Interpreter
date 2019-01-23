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

  interpreter.addEventListener([&repl](LogLevel level, std::string input){repl.print(level, input);});
  repl.addEventListener([&interpreter, &parser, &repl](std::string input) {
    try {
      std::vector<ast::Statement*> stmts = parser.parse(input);
      interpreter.exec(stmts);
    }
    catch (ExpectMore e) {
      repl.requestMore();
    };
  });

  repl.start();
}

