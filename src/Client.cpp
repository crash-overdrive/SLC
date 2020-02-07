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
  (void)Stream;
  (void)Output;
  return true;
}

void Client::setParser(Parse::DFA &Parser) {
  this->Parser = &Parser;
}

void Client::setOutput(std::ostream &Stream) {
  Output = &Stream;
}
