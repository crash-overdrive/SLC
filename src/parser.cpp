#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "config.hpp"

bool Parser::done() {
	std::ifstream file;
	file.open(lr1_file);
	file.close();
	return false;
}
