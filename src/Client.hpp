#include "ParseDFA.hpp"
#include <functional>

class Client {
public:
  Client();
  bool preProcess(std::istream &Stream);
  bool scanOnly(std::istream &Stream);
  bool parseOnly(std::istream &Stream);
  void setParser(Parse::DFA &Parser);
  void setOutput(std::ostream &Stream);

private:
  std::ostream *Output;
  Parse::DFA *Parser;
};
