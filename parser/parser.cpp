#include "./parser.hpp"

#include <sstream>
#include "parser/scanner.hpp"

void Parser::myParserOutput::addStatement(ast::Statement* stmt)
{
  stmts.push_back(stmt);
}

std::vector<ast::Statement*> Parser::myParserOutput::getStatements()
{
  return stmts;
}

std::vector<ast::Statement*> Parser::parse(const std::string& line) const
{
  myParserOutput out;
  std::istringstream in(line);
  yy::scanner scanner(&in);
  yy::parser parser(&scanner, &out);
  int result = parser.parse();
  if (result != 0) {
    throw std::runtime_error("Unknown parsing error");
  }
  return out.getStatements();
}

