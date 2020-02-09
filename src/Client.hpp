#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ParseDFA.hpp"
#include "Weeder.hpp"

namespace Client {
  bool preProcess(std::istream &Stream);
  bool scan(std::istream &IStream);
  bool scan(std::istream &IStream, std::ostream &OStream);
  bool parse(Parse::DFA &Parser, std::istream &IStream);
  bool parse(Parse::DFA &Parser, std::istream &IStream, std::ostream &OStream);
  bool weed(Parse::Tree &Tree);
};

#endif // CLIENT_HPP
