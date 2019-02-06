#include <iostream>
#include <vector>
#include <fstream>
#include "parser/parser.hpp"
#include "repl/repl.hpp"
#include "interpreter/interpreter.hpp"

int main (int argc, char *argv[])
{
  Interpreter interpreter;
  REPL repl;
  Parser parser;

  auto loadFile = [&interpreter, &repl, &parser] (const std::string& path)
  {
    std::ifstream file;
    file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
      file.open(path, std::ifstream::in);
      std::vector<ast::Statement*> stmts = parser.parse(&file);
      interpreter.exec(stmts);
    }
    catch (std::ifstream::failure ex) {
      repl.print(Error, "Failed to open file \"" + path + "\"");
    }
    catch (yy::parser::syntax_error err) {
      repl.print(Error, err.what());
    }
    catch (std::invalid_argument ex) {
      repl.print(Error, ex.what());
    }
  };


  parser.addEventListener([&repl](int indent) {
    repl.setIndent(indent);
  });
  interpreter.addLogListener([&repl](LogLevel level, std::string input){repl.print(level, input);});
  interpreter.addLoadFileListener(loadFile);
  repl.addEventListener([&interpreter, &parser, &repl](std::string input) {
    try {
      std::vector<ast::Statement*> stmts = parser.parse(input);
      interpreter.exec(stmts);
    }
    catch (yy::parser::syntax_error err) {
      repl.print(Error, err.what());
    }
    catch (std::invalid_argument ex) {
      repl.print(Error, ex.what());
    }
  });

  if (argc > 1) {
    loadFile(argv[1]);
  }
  else {
    repl.start();
  }
}

