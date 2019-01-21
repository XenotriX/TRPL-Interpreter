#pragma once

#include <vector>
#include "./ast.hpp"
#include <functional>
#include <map>
#include <variant>
#include "../repl/repl.hpp"

class Undefined {};

typedef std::variant<
    double,
    std::string,
    bool,
    std::vector<ast::Expression*>,
    std::vector<ast::Property*>,
    Undefined
  > Value;

class Interpreter
{
public:
  void exec(ast::Statement*);
  void exec(std::vector<ast::Statement*>);
  void addEventListener(std::function<void (LogLevel, std::string)>);
private:
  void log(LogLevel, std::string);
  std::vector<std::function<void (LogLevel, std::string)>> listeners;
  std::map<std::string, ast::Expression*> variables;
  Value eval(ast::Expression*);
};

