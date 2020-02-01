#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "config.hpp"

bool Parser::done() {
	std::ifstream file;
	file.open(library_directory + "/grammar/sample.cfg");
	file.close();
	return false;
}
