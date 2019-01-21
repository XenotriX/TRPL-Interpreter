#include "./interpreter.hpp"
#include <iostream>

void Interpreter::exec(std::vector<ast::Statement*> stmts)
{
  for (ast::Statement* stmt: stmts) {
    switch(stmt->type) {
      case ast::VarDeclaration_t:
      {
        auto var = static_cast<ast::VarDeclaration*>(stmt);
        if (variables.count(var->name->id)) {
          log(var->name->id + " already exists.");
          return;
        }
        variables.insert({var->name->id, var->init});
        break;
      }
      case ast::Assignment_t:
      {
        auto assig = static_cast<ast::Assignment*>(stmt);
        if (!variables.count(assig->name->id)) {
          log("No variable named " + assig->name->id);
          return;
        }
        variables.at(assig->name->id) = assig->value;
        break;
      }
      case ast::PrintStatement_t:
      {
        auto print = static_cast<ast::PrintStatement*>(stmt);
        std::vector<ast::Expression*> list = print->list;
        for (auto expr: list) {
          Value val = eval(expr);
          if (std::holds_alternative<double>(val)) log(std::to_string(std::get<double>(val)));
          else if (std::holds_alternative<std::string>(val)) log(std::get<std::string>(val));
        }
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

Value Interpreter::eval(ast::Expression* expr)
{
  switch(expr->dtype) {
    case ast::Identifier_t:
    {
      std::string id = static_cast<ast::Identifier*>(expr)->id;
      if (!variables.count(id)) {
        log("No variable named " + id);
        return Undefined();
      }
      return eval(variables.at(id));
      break;
    }
    case ast::Undefined_t:
      break;
    case ast::Number_t:
      return static_cast<ast::NumberLiteral*>(expr)->value;
      break;
    case ast::String_t:
      return static_cast<ast::StringLiteral*>(expr)->value;
      break;
    case ast::Object_t:
      break;
    case ast::Array_t:
      break;
    case ast::Addition_t:
    {
      ast::Addition* addition = static_cast<ast::Addition*>(expr);
      Value left  = eval(addition->left);
      Value right = eval(addition->right);
      if (!std::holds_alternative<double>(left) ||
          !std::holds_alternative<double>(right))
        return Undefined();
      return std::get<double>(left) +  std::get<double>(right);
      break;
    }
    case ast::Substraction_t:
    {
      ast::Substraction* substraction = static_cast<ast::Substraction*>(expr);
      Value left  = eval(substraction->left);
      Value right = eval(substraction->right);
      if (!std::holds_alternative<double>(left) ||
          !std::holds_alternative<double>(right))
        return Undefined();
      return std::get<double>(left) -  std::get<double>(right);
      break;
    }
    case ast::Multiplication_t:
    {
      ast::Multiplication* multiplication = static_cast<ast::Multiplication*>(expr);
      Value left  = eval(multiplication->left);
      Value right = eval(multiplication->right);
      if (!std::holds_alternative<double>(left) ||
          !std::holds_alternative<double>(right))
        return Undefined();
      return std::get<double>(left) *  std::get<double>(right);
      break;
    }
    case ast::Division_t:
    {
      ast::Division* division = static_cast<ast::Division*>(expr);
      Value left  = eval(division->left);
      Value right = eval(division->right);
      if (!std::holds_alternative<double>(left) ||
          !std::holds_alternative<double>(right))
        return Undefined();
      return std::get<double>(left) /  std::get<double>(right);
      break;
    }
  }
}
