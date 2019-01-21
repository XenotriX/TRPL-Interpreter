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

void REPL::addEventListener(void (*callback)(std::string))
{
  listeners.push_back(callback);
}

void REPL::print(std::string output)
{
  std::cout << "=> " << output << std::endl;
}

void REPL::dispatchEvent(std::string input)
{
  for(auto listener: listeners)
    listener(input);
}
