#include "./ast.hpp"
#include <iostream>

using namespace ast;

Statement::Statement(const Type& type) :
  type(type)
{

}

ReturnStatement::ReturnStatement(Expression* value) :
  value(value), Statement(Return_t)
{

}

WhileStatement::WhileStatement(
    Expression* condition, std::vector<Statement*> consequence) :
      condition(condition), consequence(consequence), Statement(While_t)
{

}

Branch::Branch(Expression* condition, Statement* consequence) :
  Statement(Branch_t), condition(condition)
{
  this->consequence.push_back(consequence);
}

Branch::Branch(Expression* condition, std::vector<Statement*> consequence) :
  Statement(Branch_t), condition(condition), consequence(consequence)
{

}

Operation::Operation(const DataType& type, Expression* left, Expression* right) :
  Expression(type), left(left), right(right)
{

}

Addition::Addition(Expression* left, Expression* right) :
  Operation(Addition_t, left, right)
{

}

Substraction::Substraction(Expression* left, Expression* right) :
  Operation(Substraction_t, left, right)
{

}

Multiplication::Multiplication(Expression* left, Expression* right) :
  Operation(Multiplication_t, left, right)
{

}

Division::Division(Expression* left, Expression* right) :
  Operation(Division_t, left, right)
{

}

Undefined::Undefined() :
  Expression(Undefined_t)
{

}

Identifier::Identifier(const std::string id) :
  id(id), Expression(Identifier_t)
{

}

Pattern::Pattern(Expression* object, Expression* member) :
  object(object), member(member), Expression(Pattern_t)
{

}

Property::Property(Identifier* key, Expression* value) :
  key(key), value(value)
{

}

PrintStatement::PrintStatement(std::vector<Expression*> list) :
  list(list), Statement(PrintStatement_t)
{

}

VarDeclaration::VarDeclaration(Identifier* name) :
  VarDeclaration(name, new Undefined())
{

}

VarDeclaration::VarDeclaration(Identifier* name, Expression* init) :
  name(name), init(init), Statement(VarDeclaration_t)
{

}

Assignment::Assignment(Identifier* name, Expression* value) :
  name(name), value(value), Statement(Assignment_t)
{

}

Expression::Expression(const DataType& dtype) :
  dtype(dtype), Statement(Expression_t)
{

}

NumberLiteral::NumberLiteral(const double value) :
  value(value), Expression(Number_t)
{

}

StringLiteral::StringLiteral(const std::string value) :
  value(value), Expression(String_t)
{

}

BooleanLiteral::BooleanLiteral(const bool value) :
  value(value), Expression(Boolean_t)
{

}

ArrayLiteral::ArrayLiteral(const std::vector<Expression*>& values) :
  values(values), Expression(Array_t)
{

}

ObjectLiteral::ObjectLiteral(const std::vector<Property*>& properties) :
  properties(properties), Expression(Object_t)
{

}

ExitStatement::ExitStatement() :
  Statement(ExitStatement_t)
{

}

using ParamList = std::vector<ast::Identifier*>;

FunctionLiteral::FunctionLiteral(ParamList params, std::vector<ast::Statement*> body) :
  params(params), body(body), Expression(Function_t)
{

}

CallStatement::CallStatement(Identifier* function, std::vector<Expression*> args) :
  function(function), args(args), Expression(Call_t)
{

}

