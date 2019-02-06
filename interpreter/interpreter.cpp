#include "./interpreter.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

static ast::Expression* val2expr(const Value& value)
{
  // Number
  if (std::holds_alternative<double>(value))
    return new ast::NumberLiteral(std::get<double>(value));
  // String
  if (std::holds_alternative<std::string>(value))
    return new ast::StringLiteral(std::get<std::string>(value));
  // Array
  if (std::holds_alternative<std::vector<ast::Expression*>>(value))
    return new ast::ArrayLiteral(std::get<std::vector<ast::Expression*>>(value));
  // Object
  if (std::holds_alternative<std::unordered_map<std::string, ast::Expression*>>(value))
    throw std::logic_error("Not Implemented");
  // Undefined
  if (std::holds_alternative<Undefined>(value))
    return new ast::Undefined();
  // Function
  if (std::holds_alternative<ast::FunctionLiteral>(value))
    return new ast::FunctionLiteral(std::get<ast::FunctionLiteral>(value));
}

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
    case ast::StmtType::VarDeclaration:
    {
      auto var = static_cast<ast::VarDeclaration*>(stmt);
      try {
        if (var->byValue)
          storage.Put(var->name->id, val2expr(eval(var->init)));
        else
          storage.Put(var->name->id, var->init);
      } catch (std::invalid_argument ex) {
        log(Error, ex.what());
      }
      break;
    }
    case ast::StmtType::Assignment:
    {
      auto assig = static_cast<ast::Assignment*>(stmt);
      try {
        if (assig->byValue)
          storage.Update(assig->name->id, val2expr(eval(assig->value)));
        else
          storage.Update(assig->name->id, assig->value);
      } catch (std::invalid_argument ex) {
        log(Error, ex.what());
      }
      break;
    }
    case ast::StmtType::Print:
    {
      auto print = static_cast<ast::PrintStatement*>(stmt);
      std::vector<ast::Expression*> list = print->list;
      for (auto expr: list) {
        Value val = eval(expr);
        log(Info, toString(val));
      }
      break;
    }
    case ast::StmtType::Branch:
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
    case ast::StmtType::While:
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
    case ast::StmtType::Expression:
      log(Info, toString(eval(static_cast<ast::Expression*>(stmt))));
      break;
    case ast::StmtType::Return:
      return eval(static_cast<ast::ReturnStatement*>(stmt)->value);
      break;
    case ast::StmtType::Exit:
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
  else if (std::holds_alternative<std::string>(val)) return "\"" + std::get<std::string>(val) + "\"";
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
    case ast::ExprType::Identifier:
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
    case ast::ExprType::Pattern:
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
        if (pattern->member->dtype != ast::ExprType::Identifier)
          return Undefined();
        auto properties = std::get<std::unordered_map<std::string, ast::Expression*>>(object);
        return eval(properties.at(static_cast<ast::Identifier*>(pattern->member)->id));
      }
      break;
    }
    case ast::ExprType::Number:
      return static_cast<ast::NumberLiteral*>(expr)->value;
      break;
    case ast::ExprType::String:
      return static_cast<ast::StringLiteral*>(expr)->value;
      break;
    case ast::ExprType::Boolean:
      return static_cast<ast::BooleanLiteral*>(expr)->value;
      break;
    case ast::ExprType::Object:
    {
      std::unordered_map<std::string, ast::Expression*> dict;
      auto properties = static_cast<ast::ObjectLiteral*>(expr)->properties;
      for (auto property: properties) {
        dict.insert({property->key->id, property->value});
      }
      return dict;
      break;
    }
    case ast::ExprType::Array:
      return static_cast<ast::ArrayLiteral*>(expr)->values;
      break;
    case ast::ExprType::Operation:
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
    case ast::ExprType::Function:
    {
      auto funcLiteral = static_cast<ast::FunctionLiteral*>(expr);
      return *funcLiteral;
      break;
    }
    case ast::ExprType::Call:
    {
      auto call = static_cast<ast::CallStatement*>(expr);
      Value value = eval(call->function);
      if (!std::holds_alternative<ast::FunctionLiteral>(value))
        throw std::invalid_argument("\"" + call->function->id + "\" is not a function");
      auto function = std::get<ast::FunctionLiteral>(value);
      auto args = call->args;
      for (auto& arg: args)
        arg = val2expr(eval(arg));
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
    case ast::ExprType::TypeOf:
    {
      auto typeOf = static_cast<ast::TypeOf*>(expr);
      Value value = eval(typeOf->expr);
      if (std::holds_alternative<double>(value))
        return std::string("Number");
      if (std::holds_alternative<std::string>(value))
        return std::string("String");
      if (std::holds_alternative<bool>(value))
        return std::string("Boolean");
      if (std::holds_alternative<std::vector<ast::Expression*>>(value))
        return std::string("Array");
      if (std::holds_alternative<std::unordered_map<std::string, ast::Expression*>>(value))
        return std::string("Object");
      if (std::holds_alternative<Undefined>(value))
        return std::string("Undefined");
      if (std::holds_alternative<ast::FunctionLiteral>(value))
        return std::string("Function");
      break;
    }
    default:
    case ast::ExprType::Undefined:
      return Undefined();
      break;
  }
  return Undefined();
}

