#pragma once

#include <string>
#include <vector>
#include <map>

namespace ast
{
  enum Type {
    PrintStatement_t,
    VarDeclaration_t,
    Assignment_t,
    Branch_t,
    ExitStatement_t,
    Expression_t,
  };

  enum DataType {
    Identifier_t,
    Undefined_t,
    Number_t,
    String_t,
    Boolean_t,
    Object_t,
    Array_t,
    Addition_t,
    Substraction_t,
    Multiplication_t,
    Division_t,
    Pattern_t,
    Function_t,
    Call_t,
  };

  struct Statement
  {
    Statement(const Type&);
    Type type;
  };

  struct Expression : Statement
  {
    DataType dtype;
    Expression(const DataType&);
  };

  struct Pattern : Expression
  {
    Pattern(Expression* object, Expression* member);
    Expression* object;
    Expression* member;
  };

  struct Branch : Statement
  {
    Branch(Expression*, Statement*);
    Branch(Expression*, std::vector<Statement*>);
    Expression* condition;
    std::vector<Statement*> consequence;
  };

  struct Operation : Expression
  {
    Operation(const DataType&, Expression*, Expression*);
    Expression* left;
    Expression* right;
  };

  struct Addition : Operation
  {
    Addition(Expression*, Expression*);
  };

  struct Substraction : Operation
  {
    Substraction(Expression*, Expression*);
  };

  struct Multiplication : Operation
  {
    Multiplication(Expression*, Expression*);
  };

  struct Division : Operation
  {
    Division(Expression*, Expression*);
  };

  struct Undefined : Expression
  {
    Undefined();
  };

  struct Identifier : Expression
  {
    Identifier(const std::string);
    std::string id;
  };

  struct PrintStatement : Statement
  {
    PrintStatement(std::vector<Expression*>);
    std::vector<Expression*> list;
  };

  struct VarDeclaration : Statement
  {
    VarDeclaration(Identifier*);
    VarDeclaration(Identifier*, Expression*);
    Identifier* name;
    Expression* init;
  };

  struct Property
  {
    Property(Identifier*, Expression*);
    Identifier* key;
    Expression* value;
  };

  struct Assignment : Statement
  {
    Assignment(Identifier*, Expression*);
    Identifier* name;
    Expression* value;
  };

  struct NumberLiteral : Expression
  {
    NumberLiteral(const double);
    double value;
  };

  struct StringLiteral : Expression
  {
    StringLiteral(const std::string);
    std::string value;
  };

  struct BooleanLiteral : Expression
  {
    BooleanLiteral(const bool);
    bool value;
  };

  struct ArrayLiteral : Expression
  {
    ArrayLiteral(const std::vector<Expression*>& values);
    std::vector<Expression*> values;
  };

  struct ObjectLiteral : Expression
  {
    ObjectLiteral(const std::vector<Property*>& properties);
    std::vector<Property*> properties;
  };

  struct ExitStatement : Statement
  {
    ExitStatement();
  };

  using ParamList = std::vector<ast::Identifier*>;

  struct FunctionLiteral : Expression
  {
    FunctionLiteral(ParamList params, std::vector<Statement*> body);
    ParamList params;
    std::vector<Statement*> body;
  };

  struct CallStatement : Expression
  {
    CallStatement(Identifier* function, std::vector<Expression*> args);
    Identifier* function;
    std::vector<Expression*> args;
  };
}
