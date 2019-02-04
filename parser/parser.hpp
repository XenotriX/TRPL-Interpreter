#pragma once

#include "parser.hxx"
#include <functional>

class Parser
{
public:
  std::vector<ast::Statement*> parse(const std::string&) const;
  std::vector<ast::Statement*> parse(std::istream* in) const;
  void addEventListener(std::function<void (int)>);
private:
  void requestMore(int) const;
  std::vector<std::function<void (int)>> listeners;
  struct myParserOutput : ParserOutput
  {
    std::vector<ast::Statement*> stmts;
    void addStatement(ast::Statement* stmt) override;
    std::vector<ast::Statement*> getStatements();
  };
};

