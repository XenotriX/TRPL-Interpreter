#include "./ast.hpp"
#include <iostream>

using namespace ast;

VarDeclaration::VarDeclaration(const std::string& name) :
  name(name)
{
  std::cout << name << std::endl;
}

VarDeclaration::VarDeclaration(const std::string&, const std::string&) :
  name(name), init(init)
{

}
