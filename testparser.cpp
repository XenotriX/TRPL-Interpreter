#include <iostream>
#include <vector>
#include "parser.hxx"

struct myParserOutput : ParserOutput
{
  std::vector<ast::Statement*> stmts;

  void addStatement(ast::Statement* stmt) override
  {
    stmts.push_back(stmt);
  }

  std::vector<ast::Statement*> getStatements()
  {
    return stmts;
  }
};

int main (int argc, char *argv[])
{
  myParserOutput cb;
  std::vector<std::string> lines = {
    "var test\n",
    "test = 5.3\n",
    "print test, asdf, fr\n",
    "var myVar = {\nkey: \"val\"\n}\n"
  };
  parse(lines, &cb);
  auto stmts = cb.getStatements();
  for (ast::Statement* stmt: stmts) {
    switch(stmt->type) {
      case ast::VarDeclaration_t:
      {
        auto var = static_cast<ast::VarDeclaration*>(stmt);
        std::cout << "Variable Declaration" << std::endl;
        std::cout << "->Name: " << static_cast<ast::Identifier*>(var->name)->id << std::endl;
        auto expr = static_cast<ast::Expression*>(var->init);
        std::cout << "Address is: " << expr << std::endl;
        switch(expr->dtype) {
          case ast::Number_t:
          {
            std::cout << "->Initial value: " << static_cast<ast::NumberLiteral*>(var->init)->value << std::endl;
            break;
          }
          case ast::Object_t:
          {
            std::cout << "->Initial value: " << "{Object}" << std::endl;
            break;
          }
          default:
            std::cout << "Not Implemented" << std::endl;
            break;
        }
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

