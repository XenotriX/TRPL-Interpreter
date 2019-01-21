#pragma once

#include <vector>
#include "./ast.hpp"
#include <functional>
#include <map>
#include <variant>

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
  void addEventListener(std::function<void (std::string)>);
private:
  void log(std::string);
  std::vector<std::function<void (std::string)>> listeners;
  std::map<std::string, ast::Expression*> variables;
  Value eval(ast::Expression*);
};

