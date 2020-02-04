#include <iostream>
#include <fstream>
#include "Parser.hpp"
#include "Config.hpp"

bool Parser::done() {
  std::ifstream file;
  file.open(lr1_file);
  file.close();
  return false;
}
