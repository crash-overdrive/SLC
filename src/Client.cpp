#include "Client.hpp"

bool Client::preProcess(std::istream &IStream, const std::string &FileName) {
  (void)IStream;
  (void)FileName;
  return true;
}

bool Client::verifyFileName(const std::string &FileName) {
  const std::string Ext(".java");
  if (FileName.length() < Ext.length()) {
    return false;
  }
  size_t Position = FileName.find(".");
  return FileName.compare(Position, Ext.size(), Ext) == 0;
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
  Lex::Token Tok;
  while (IStream >> Tok) {
    Parser.read(Tok);
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
  Parse::Tree T = Parser.buildTree();
  if (flag) {
    OStream << T << std::endl;
  } else {
    OStream << "Error: Input does not conformed to joos.cfg\n";
    OStream << T << std::endl;
    return false;
  }
  return weed(T, "");
}

bool Client::weed(const Parse::Tree &Tree, const std::string &Name) {
  (void)Name;
  for (const auto &Check : Weed::JoosChecks) {
    if (!Check(Tree)) {
      return false;
    }
  }
  return true;
}
