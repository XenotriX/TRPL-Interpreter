#include "./interpreter.hpp"
#include <stdexcept>

using Frame = std::map<std::string, ast::Expression*>;
using Stack = std::vector<Frame>;

Storage::Storage()
{
  NewFrame();
}

ast::Expression* Storage::Get(const std::string& name) const
{
  if (!stack.front().count(name)) {
    throw std::invalid_argument("No Variable named \"" + name + "\"");
  }
  return stack.front().at(name);
}

void Storage::Put(const std::string& name, ast::Expression* value)
{
  if (stack.front().count(name)) {
    throw std::invalid_argument("Variable \"" + name + "\" already exists");
  }
  stack.front().insert({name, value});
}

void Storage::Update(const std::string& name, ast::Expression* value)
{
  if (!stack.front().count(name)) {
    throw std::invalid_argument("No Variable named \"" + name + "\"");
  }
  stack.front().at(name) = value;
}

void Storage::NewFrame()
{
  stack.emplace({});
}

void Storage::PopFrame()
{
  if (stack.size() <= 1)
    throw std::runtime_error("Base frame cannot be popped");
  stack.pop_back();
}
