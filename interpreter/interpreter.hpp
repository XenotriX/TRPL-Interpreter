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
    Undefined,
    ast::FunctionLiteral
  > Value;

class Interpreter
{
public:
  Value exec(ast::Statement*) const;
  Value exec(std::vector<ast::Statement*>) const;
  void addLogListener(std::function<void (LogLevel, std::string)>);
  void addLoadFileListener(std::function<void (std::string)>);
private:
  void log(LogLevel, std::string) const;
  std::string toString(const Value& value) const;
  Value eval(ast::Expression*) const;
  void loadFile(const std::string& path) const;
  std::vector<std::function<void (LogLevel, std::string)>> logListeners;
  std::vector<std::function<void (std::string)>> loadFileListeners;
  mutable Storage storage;
};

