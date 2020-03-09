#include "Client.hpp"
#include "Config.hpp"
#include <fstream>

Client createClient() {
  std::unique_ptr<Lex::Scanner> Scanner = std::make_unique<Lex::Scanner>();
  std::ifstream ScannerStream;
  ScannerStream.open(TokensLexFile);
  ScannerStream >> *Scanner;

  std::unique_ptr<Parse::DFA> Parser = std::make_unique<Parse::DFA>();
  std::ifstream parserStream;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> *Parser;

  return Client(std::move(Scanner), std::move(Parser));
}
