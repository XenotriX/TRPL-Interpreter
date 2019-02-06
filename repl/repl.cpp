#include "repl.hpp"
#include <iostream> 

void REPL::start()
{
  isRunning = true;
  indent = 0;

  std::string buffer;
  std::string input;

  while (isRunning) {
    if (indent > 0) {
      for (int i = 0; i < indent; i++)
        std::cout << "--";
      std::cout << ">> ";
      std::getline(std::cin, input);
      buffer += input;
      indent--;
    } else {
      std::cout << ">> ";
      std::getline(std::cin, buffer);
    }
    dispatchEvent(buffer += "\n");
  }
}

void REPL::addEventListener(std::function<void (std::string)> callback)
{
  listeners.push_back(callback);
}

void REPL::print(LogLevel level, std::string output)
{
  switch(level) {
    case Info:
      std::cout << "=> " << output << std::endl;
      break;
    case Warning:
      std::cout << "!> " << output << std::endl;
      break;
    case Error:
      std::cout << "x> " << output << std::endl;
      break;
  }
}

void REPL::setIndent(int indent)
{
  this->indent = indent;
};

void REPL::dispatchEvent(std::string input)
{
  for(auto listener: listeners)
    listener(input);
}
