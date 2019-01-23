#pragma once

#include "parser.hxx"

class Parser
{
public:
  std::vector<ast::Statement*> parse(const std::string&) const;
private:
  struct myParserOutput : ParserOutput
  {
    std::vector<ast::Statement*> stmts;
    void addStatement(ast::Statement* stmt) override;
    std::vector<ast::Statement*> getStatements();
  };
};

