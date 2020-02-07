#include "ParseDFA.hpp"

class Client {
public:
  bool preProcess(std::istream &Stream);
  bool scan(std::istream &IStream);
  bool scan(std::istream &IStream, std::ostream &OStream);
  bool parse(Parse::DFA &Parser, std::istream &IStream);
  bool parse(Parse::DFA &Parser, std::istream &IStream, std::ostream &OStream);
};
