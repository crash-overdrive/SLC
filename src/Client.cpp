#include "Client.hpp"

bool Client::preProcess(std::istream &IStream) {
  (void)IStream;
  return true;
}

bool Client::scan(std::istream &IStream) {
  (void)IStream;
  return true;
}

bool Client::scan(std::istream &IStream, std::ostream &OStream) {
  (void)IStream;
  (void)OStream;
  return true;
}

bool Client::parse(Parse::DFA &Parser, std::istream &IStream) {
  std::string Token;
  while (IStream >> Token) {
    Parser.read(Token);
    if (Parser.error()) {
      return false;
    }
    IStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return Parser.accept();
}

bool Client::parse(Parse::DFA &Parser, std::istream &IStream,
                   std::ostream &OStream) {
  bool flag = parse(Parser, IStream);
  if (flag) {
    OStream << Parser.buildTree();
  }
  return flag;
}
