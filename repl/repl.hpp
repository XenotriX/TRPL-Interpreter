#pragma once

#include <string>
#include <vector>
#include <functional>

enum LogLevel {
  Info,
  Warning,
  Error
};

class REPL
{
public:
  void start();
  void addEventListener(std::function<void (std::string)> callback);
  void print(LogLevel, std::string);
  void setIndent(int);
private:
  void dispatchEvent(std::string);
  std::vector<std::function<void (std::string)>> listeners;
  bool isRunning;
  int indent;
};

