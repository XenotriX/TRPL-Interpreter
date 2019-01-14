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
  std::vector<std::string> lines;
  lines.push_back("var test");
  parse(lines, &cb);
  auto stmts = cb.getStatements();
  ast::Statement* stmt = stmts.at(0);
  ast::VarDeclaration* decl = static_cast<ast::VarDeclaration*>(stmt);
  std::cout << "name: " << decl->name << "; init: " << decl->init << std::endl;
}

