#include "./ast.hpp"
#include <iostream>

using namespace ast;

Statement::Statement(const Type& type) :
  type(type)
{
  std::cout << "creating statement of type: " << type << std::endl;
}

Undefined::Undefined() :
  Expression(Undefined_t)
{

}

Identifier::Identifier(const std::string id) :
  id(id), Expression(Identifier_t)
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
  dtype(dtype)
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

ObjectLiteral::ObjectLiteral(std::vector<Property*> properties) :
  properties(properties), Expression(Object_t)
{

}

