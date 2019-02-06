#include "./ast.hpp"
#include <iostream>

using namespace ast;

Statement::Statement(const StmtType& type) :
  type(type)
{

}

ReturnStatement::ReturnStatement(Expression* value) :
  value(value), Statement(StmtType::Return)
{

}

WhileStatement::WhileStatement(
    Expression* condition, std::vector<Statement*> consequence) :
      condition(condition), consequence(consequence), Statement(StmtType::While)
{

}

Branch::Branch(Expression* condition, Statement* consequence) :
  Statement(StmtType::Branch), condition(condition)
{
  this->consequence.push_back(consequence);
}

Branch::Branch(Expression* condition, std::vector<Statement*> consequence) :
  Statement(StmtType::Branch), condition(condition), consequence(consequence)
{

}

Operation::Operation(const Operator& op, Expression* left, Expression* right) :
  Expression(ExprType::Operation), op(op), left(left), right(right)
{

}

Undefined::Undefined() :
  Expression(ExprType::Undefined)
{

}

Identifier::Identifier(const std::string id) :
  id(id), Expression(ExprType::Identifier)
{

}

Pattern::Pattern(Expression* object, Expression* member) :
  object(object), member(member), Expression(ExprType::Pattern)
{

}

Property::Property(Identifier* key, Expression* value) :
  key(key), value(value)
{

}

PrintStatement::PrintStatement(std::vector<Expression*> list) :
  list(list), Statement(StmtType::Print)
{

}

TypeOf::TypeOf(Expression* expr) :
  expr(expr), Expression(ExprType::TypeOf)
{

}

VarDeclaration::VarDeclaration(Identifier* name) :
  VarDeclaration(name, new Undefined())
{

}

VarDeclaration::VarDeclaration(Identifier* name, Expression* init, bool byValue/*= false*/) :
  name(name), init(init), byValue(byValue), Statement(StmtType::VarDeclaration)
{

}

Assignment::Assignment(Identifier* name, Expression* value, bool byValue /*= false*/) :
  name(name), value(value), byValue(byValue), Statement(StmtType::Assignment)
{

}

Expression::Expression(const ExprType& dtype) :
  dtype(dtype), Statement(StmtType::Expression)
{

}

NumberLiteral::NumberLiteral(const double value) :
  value(value), Expression(ExprType::Number)
{

}

StringLiteral::StringLiteral(const std::string value) :
  value(value), Expression(ExprType::String)
{

}

BooleanLiteral::BooleanLiteral(const bool value) :
  value(value), Expression(ExprType::Boolean)
{

}

ArrayLiteral::ArrayLiteral(const std::vector<Expression*>& values) :
  values(values), Expression(ExprType::Array)
{

}

ObjectLiteral::ObjectLiteral(const std::vector<Property*>& properties) :
  properties(properties), Expression(ExprType::Object)
{

}

ExitStatement::ExitStatement() :
  Statement(StmtType::Exit)
{

}

LoadStatement::LoadStatement(Expression* path) :
  path(path), Statement(StmtType::Load)
{

}

using ParamList = std::vector<ast::Identifier*>;

FunctionLiteral::FunctionLiteral(ParamList params, std::vector<ast::Statement*> body) :
  params(params), body(body), Expression(ExprType::Function)
{

}

CallStatement::CallStatement(Identifier* function, std::vector<Expression*> args) :
  function(function), args(args), Expression(ExprType::Call)
{

}

