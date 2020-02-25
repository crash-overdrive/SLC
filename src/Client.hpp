#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ParseDFA.hpp"

namespace Client {
  bool preProcess(std::istream &Stream, const std::string &FileName);
  bool verifyFileName(const std::string &FileName);
};

#endif // CLIENT_HPP
