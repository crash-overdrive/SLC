#include "Client.hpp"

Client::Client() : Output(nullptr), Parser(nullptr) {}

bool Client::preProcess(std::istream &Stream) {
  (void)Stream;
  return true;
}

bool Client::scanOnly(std::istream &Stream) {
  (void)Stream;
  return true;
}

bool Client::parseOnly(std::istream &Stream) {
  std::string Token;
  while (Stream >> Token) {
    Parser->read(Token);
    if (Parser->error()) {
      return false;
    }
    Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return Parser->accept();
}

void Client::setParser(Parse::DFA &Parser) {
  this->Parser = &Parser;
}

void Client::setOutput(std::ostream &Stream) {
  Output = &Stream;
}
