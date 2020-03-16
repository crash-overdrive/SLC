#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP

#include "Client.hpp"

Client createClient();

std::vector<std::string>
createMarmosetTest(unsigned int num, const std::vector<std::string> &fileNames);

#endif // TESTUTIL_HPP
