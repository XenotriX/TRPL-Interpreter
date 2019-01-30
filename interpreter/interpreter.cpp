#include "./interpreter.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

Value Interpreter::exec(std::vector<ast::Statement*> stmts) const
{
  for (ast::Statement* stmt: stmts) {
    Value val = exec(stmt);
    if (!std::holds_alternative<Undefined>(val))
      return val;
  }
  return Undefined();
}

Value Interpreter::exec(ast::Statement* stmt) const
{
  switch(stmt->type) {
    case ast::VarDeclaration_t:
    {
      auto var = static_cast<ast::VarDeclaration*>(stmt);
      try {
        storage.Put(var->name->id, var->init);
      } catch (std::invalid_argument ex) {
        log(Error, ex.what());
      }
      break;
    }
    case ast::Assignment_t:
    {
      auto assig = static_cast<ast::Assignment*>(stmt);
      try {
        storage.Update(assig->name->id, assig->value);
      } catch (std::invalid_argument ex) {
        log(Error, ex.what());
      }
      break;
    }
    case ast::PrintStatement_t:
    {
      auto print = static_cast<ast::PrintStatement*>(stmt);
      std::vector<ast::Expression*> list = print->list;
      for (auto expr: list) {
        Value val = eval(expr);
        log(Info, toString(val));
      }
      break;
    }
    case ast::Branch_t:
    {
      auto branch = static_cast<ast::Branch*>(stmt);
      Value condition = eval(branch->condition);
      if (!std::holds_alternative<bool>(condition)) {
        log(Error, "Condition is not of type Boolean.");
        break;
      }
      if (std::get<bool>(condition)){
        exec(branch->consequence);
      }
      break;
    }
    case ast::While_t:
    {
      auto whileStmt = static_cast<ast::WhileStatement*>(stmt);
      if (!std::holds_alternative<bool>(eval(whileStmt->condition))) {
        log(Error, "Condition is not of type Boolean.");
        break;
      }
      while (std::get<bool>(eval(whileStmt->condition))) {
        exec(whileStmt->consequence);
        if (!std::holds_alternative<bool>(eval(whileStmt->condition))) {
          log(Error, "Condition is no longer of type Boolean");
          break;
        }
      }
      break;
    }
    case ast::Expression_t:
      log(Info, toString(eval(static_cast<ast::Expression*>(stmt))));
      break;
    case ast::Return_t:
      return eval(static_cast<ast::ReturnStatement*>(stmt)->value);
      break;
    case ast::ExitStatement_t:
      log(Warning, "Terminating");
      exit(0);
      break;
    default:
      std::cout << "Not Implemented" << std::endl;
      break;
  }
  return Undefined();
}

std::string Interpreter::toString(const Value& val) const
{
  if (std::holds_alternative<double>(val)) {
    double number = std::get<double>(val);
    if (std::floor(number) == number)
      return std::to_string((int)number);
    return std::to_string(number);
  }
  else if (std::holds_alternative<std::string>(val)) return std::get<std::string>(val);
  else if (std::holds_alternative<bool>(val)) return std::get<bool>(val) ? "true" : "false";
  else if (std::holds_alternative<Undefined>(val)) return "Undefined";
  else if (std::holds_alternative<std::vector<ast::Expression*>>(val)) {
    auto array = std::get<std::vector<ast::Expression*>>(val);
    std::string str = "[ ";
    for (auto value: array) {
      if (value != array.front())
        str += ", ";
      str += toString(eval(value));
    }
    str += " ]";
    return str;
  }
  else if (std::holds_alternative<std::unordered_map<std::string, ast::Expression*>>(val)) {
    auto dict = std::get<std::unordered_map<std::string, ast::Expression*>>(val);
    std::string str = "{ ";
    for (auto prop: dict) {
      if (prop != *dict.begin())
        str += ", ";
      str += prop.first + ": " + toString(eval(prop.second));
    }
    str += " }";
    return str;
  }
  else {
    log(Warning, "Expression cannot be converted to a string");
    return "";
  }
}

void Interpreter::addEventListener(std::function<void (LogLevel, std::string)> callback)
{
  listeners.push_back(callback);
}

void Interpreter::log(LogLevel level, std::string output) const
{
  for (auto listener: listeners)
    listener(level, output);
}

Value Interpreter::eval(ast::Expression* expr) const
{
  switch(expr->dtype) {
    case ast::Identifier_t:
    {
      std::string id = static_cast<ast::Identifier*>(expr)->id;
      try {
        return eval(storage.Get(id));
      } catch (std::invalid_argument ex) {
        log(Error, ex.what());
        return Undefined();
      }
      break;
    }
    case ast::Pattern_t:
    {
      auto pattern = static_cast<ast::Pattern*>(expr);
      Value object = eval(pattern->object);
      if (std::holds_alternative<std::vector<ast::Expression*>>(object)) {
        Value member = eval(pattern->member);
        if (!std::holds_alternative<double>(member)) {
          log(Error, "Index must be a number");
          return Undefined();
        }
        auto expression = std::get<std::vector<ast::Expression*>>(object);
        return eval(expression.at((int)std::get<double>(member)));
      }
      else if (std::holds_alternative<std::unordered_map<std::string, ast::Expression*>>(object)) {
        if (pattern->member->dtype != ast::Identifier_t)
          return Undefined();
        auto properties = std::get<std::unordered_map<std::string, ast::Expression*>>(object);
        return eval(properties.at(static_cast<ast::Identifier*>(pattern->member)->id));
      }
      break;
    }
    case ast::Number_t:
      return static_cast<ast::NumberLiteral*>(expr)->value;
      break;
    case ast::String_t:
      return static_cast<ast::StringLiteral*>(expr)->value;
      break;
    case ast::Boolean_t:
      return static_cast<ast::BooleanLiteral*>(expr)->value;
      break;
    case ast::Object_t:
    {
      std::unordered_map<std::string, ast::Expression*> dict;
      auto properties = static_cast<ast::ObjectLiteral*>(expr)->properties;
      for (auto property: properties) {
        dict.insert({property->key->id, property->value});
      }
      return dict;
      break;
    }
    case ast::Array_t:
      return static_cast<ast::ArrayLiteral*>(expr)->values;
      break;
    case ast::Operation_t:
    {
      auto operation = static_cast<ast::Operation*>(expr);
      double right, left;
      try {
        left  = std::get<double>(eval(operation->left));
        right = std::get<double>(eval(operation->right));
      }
      catch (std::bad_variant_access ex) {
        return Undefined();
      }
      switch(operation->op) {
        case ast::Operator::Addition:
          return left +  right;
        case ast::Operator::Substraction:
          return left -  right;
        case ast::Operator::Multiplication:
          return left *  right;
        case ast::Operator::Division:
          if (right == 0) throw std::invalid_argument("Division by zero");
          return left / right;
        case ast::Operator::Equal:
          return left == right;
        case ast::Operator::NotEqual:
          return left != right;
        case ast::Operator::Less:
          return left < right;
        case ast::Operator::LessEqual:
          return left <= right;
        case ast::Operator::Greater:
          return left > right;
        case ast::Operator::GreaterEqual:
          return left >= right;
      }
    }
    case ast::Function_t:
    {
      auto funcLiteral = static_cast<ast::FunctionLiteral*>(expr);
      return *funcLiteral;
      break;
    }
    case ast::Call_t:
    {
      auto call = static_cast<ast::CallStatement*>(expr);
      Value value = eval(call->function);
      if (!std::holds_alternative<ast::FunctionLiteral>(value))
        throw std::invalid_argument("\"" + call->function->id + "\" is not a function");
      auto function = std::get<ast::FunctionLiteral>(value);
      auto args = call->args;
      auto params = function.params;
      if (params.size() != args.size())
        throw std::invalid_argument("Parameters and arguments don't match");
      storage.NewFrame();
      for (int i = 0; i < params.size(); i++)
        storage.Put(params[i]->id, args[i]);
      Value ret = exec(function.body);
      storage.PopFrame();
      return ret;
      break;
    }
    default:
    case ast::Undefined_t:
      return Undefined();
      break;
  }
  return Undefined();
}

