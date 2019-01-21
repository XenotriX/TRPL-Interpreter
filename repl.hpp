#pragma once

#include <string>
#include <vector>
#include <functional>

class REPL
{
public:
  void start();
  void addEventListener(std::function<void (std::string)> callback);
  void print(std::string);
private:
  void dispatchEvent(std::string);
  std::vector<std::function<void (std::string)>> listeners;
  bool isRunning;
};

