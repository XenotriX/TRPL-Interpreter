#pragma once

#include <vector>
#include "./ast.hpp"
#include <functional>
#include <map>

class Interpreter
{
public:
  void eval(std::vector<ast::Statement*>);
  void addEventListener(std::function<void (std::string)>);
private:
  void log(std::string);
  std::vector<std::function<void (std::string)>> listeners;
  std::map<std::string, ast::Expression*> variables;
};

