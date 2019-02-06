#pragma once

#include <string>
#include <vector>
#include <map>

namespace ast
{
  enum class StmtType {
    Print,
    VarDeclaration,
    Assignment,
    Branch,
    Exit,
    Expression,
    Return,
    While,
  };

  enum class ExprType {
    Identifier,
    Undefined,
    Number,
    String,
    Boolean,
    Object,
    Array,
    Operation,
    Pattern,
    Function,
    Call,
  };

  enum class Operator {
    Addition,
    Substraction,
    Multiplication,
    Division,
    Equal,
    NotEqual,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
  };

  struct Statement
  {
    Statement(const StmtType&);
    StmtType type;
  };

  struct Expression : Statement
  {
    ExprType dtype;
    Expression(const ExprType&);
  };

  struct WhileStatement : Statement
  {
    WhileStatement(Expression* condition, std::vector<Statement*> consequence);
    Expression* condition;
    std::vector<Statement*> consequence;
  };

  struct ReturnStatement : Statement
  {
    ReturnStatement(Expression* value);
    Expression* value;
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
    Operation(const Operator& op, Expression*, Expression*);
    Operator op;
    Expression* left;
    Expression* right;
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
    VarDeclaration(Identifier* id, Expression* value, bool byValue = false);
    bool byValue;
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
    Assignment(Identifier* id, Expression* expr, bool byValue = false);
    bool byValue;
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
