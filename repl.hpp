#pragma once

#include <string>
#include <vector>

class REPL
{
public:
  void start();
  void addEventListener(void (*callback)(std::string));
  void print(std::string);
private:
  void dispatchEvent(std::string);
  std::vector<void (*)(std::string)> listeners;
  bool isRunning;
};

