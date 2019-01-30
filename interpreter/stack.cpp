#include "./interpreter.hpp"
#include <stdexcept>

using Frame = std::map<std::string, ast::Expression*>;
using Stack = std::stack<Frame>;

Storage::Storage()
{
  NewFrame();
}

ast::Expression* Storage::Get(const std::string& name) const
{
  if (!stack.top().count(name)) {
    throw std::invalid_argument("No Variable named \"" + name + "\"");
  }
  return stack.top().at(name);
}

void Storage::Put(const std::string& name, ast::Expression* value)
{
  if (stack.top().count(name)) {
    throw std::invalid_argument("Variable \"" + name + "\" already exists");
  }
  stack.top().insert({name, value});
}

void Storage::Update(const std::string& name, ast::Expression* value)
{
  if (!stack.top().count(name)) {
    throw std::invalid_argument("No Variable named \"" + name + "\"");
  }
  stack.top().at(name) = value;
}

void Storage::NewFrame()
{
  stack.push(Frame());
}

void Storage::PopFrame()
{
  if (stack.size() <= 1)
    throw std::runtime_error("Base frame cannot be popped");
  stack.pop();
}
