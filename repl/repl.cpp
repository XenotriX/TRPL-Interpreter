#include "repl.hpp"
#include <iostream> 

void REPL::start()
{
  isRunning = true;

  std::string input;

  while (isRunning) {
    std::cout << ">> ";
    std::getline(std::cin, input);
    dispatchEvent(input);
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

void REPL::dispatchEvent(std::string input)
{
  for(auto listener: listeners)
    listener(input);
}
