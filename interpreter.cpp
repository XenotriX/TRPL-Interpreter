#include <iostream>
#include "./interpreter.hpp"

void Interpreter::eval(std::vector<ast::Statement*> stmts)
{
  for (ast::Statement* stmt: stmts) {
    switch(stmt->type) {
      case ast::VarDeclaration_t:
      {
        auto var = static_cast<ast::VarDeclaration*>(stmt);
        variables.insert({var->name->id, var->init});
        break;
      }
      case ast::Assignment_t:
      {
        auto assig = static_cast<ast::Assignment*>(stmt);
        std::cout << "Assignement" << std::endl;
        std::cout << "->Left: " << static_cast<ast::Identifier*>(assig->name)->id << std::endl;
        std::cout << "->Right: " << static_cast<ast::NumberLiteral*>(assig->value)->value << std::endl;
        break;
      }
      case ast::PrintStatement_t:
      {
        auto print = static_cast<ast::PrintStatement*>(stmt);
        std::cout << "Print" << std::endl;
        std::vector<ast::Expression*> list = print->list;
        std::cout << list.size() << std::endl;
        for (auto expr: list)
          std::cout << static_cast<ast::Identifier*>(expr)->id << std::endl;
        break;
      }
      default:
        std::cout << "Not Implemented" << std::endl;
        break;
    }
  }
}

void Interpreter::addEventListener(std::function<void (std::string)> callback)
{
  listeners.push_back(callback);
}

void Interpreter::log(std::string output)
{
  for (auto listener: listeners)
    listener(output);
}
