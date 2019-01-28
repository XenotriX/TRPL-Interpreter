#pragma once

#include <map>
#include "./ast.hpp"

using Frame = std::map<std::string, ast::Expression*>;
using Stack = std::vector<Frame>;

class Storage
{
public:
  Storage();
  ast::Expression* Get(const std::string& name) const;
  void Put(const std::string& name, ast::Expression*);
  void Update(const std::string& name, ast::Expression*);
  void NewFrame();
  void PopFrame();
private:
  Stack stack;
};
