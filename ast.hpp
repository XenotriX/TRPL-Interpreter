#pragma once

#include <string>

namespace ast
{
  // Statements
  struct Statement
  {
  };

  struct VarDeclaration : public Statement
  {
    VarDeclaration() = default;
    VarDeclaration(const std::string&);
    VarDeclaration(const std::string&, const std::string&);
    std::string name;
    std::string init;
  };
}
