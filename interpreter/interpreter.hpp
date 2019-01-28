#pragma once

#include <vector>
#include "./ast.hpp"
#include <functional>
#include <map>
#include <variant>
#include "../repl/repl.hpp"
#include "stack.hpp"

class Undefined {};

typedef std::variant<
    double,
    std::string,
    bool,
    std::vector<ast::Expression*>,
    std::unordered_map<std::string, ast::Expression*>,
    Undefined
  > Value;

class Interpreter
{
public:
  void exec(ast::Statement*);
  void exec(std::vector<ast::Statement*>);
  void addEventListener(std::function<void (LogLevel, std::string)>);
private:
  void log(LogLevel, std::string) const;
  std::string toString(const Value& value) const;
  std::vector<std::function<void (LogLevel, std::string)>> listeners;
  Value eval(ast::Expression*) const;
  Storage storage;
};

