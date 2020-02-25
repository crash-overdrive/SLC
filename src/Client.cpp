#include "Client.hpp"
#include <fstream>

Client::Client()
    : Scanner(nullptr), Parser(nullptr), FileNames(), IncludeStdlib(true),
      BreakPoint(Preprocess) {}

Client::Client(Lex::Scanner &Scanner)
    : Scanner(&Scanner), Parser(nullptr), FileNames(), IncludeStdlib(true),
      BreakPoint(Scan) {}

Client::Client(Lex::Scanner &Scanner, Parse::DFA &Parser)
    : Scanner(&Scanner), Parser(&Parser), FileNames(), IncludeStdlib(true),
      BreakPoint(Environment) {}

void Client::addJavaFile(const std::string &FileName) {
  FileNames.emplace_back(FileName);
}

void Client::addJavaFiles(std::initializer_list<std::string> List) {
  FileNames.insert(FileNames.end(), List.begin(), List.end());
}

void Client::setBreakPoint(BreakPointType BreakPoint) {
  this->BreakPoint = BreakPoint;
}

void Client::setStdlib(bool IncludeStdlib) {
  this->IncludeStdlib = IncludeStdlib;
}

bool Client::process() {
  for (const auto &FileName : FileNames) {
    if (!verifyFileName(FileName)) {
      std::cerr << FileName << " has the wrong extension" << '\n';
      return false;
    };
    if (BreakPoint == Preprocess) {
      std::cerr << FileName << " is valid format" << '\n';
      continue;
    }

    std::ifstream JavaStream;
    JavaStream.open(FileName);
    if (!JavaStream.is_open()) {
      std::cerr << FileName << " could not be opened" << '\n';
      return false;
    }

    std::vector<Lex::Token> Tokens;
    if (!scan(JavaStream, Tokens))
      return false;
    if (BreakPoint == Scan)
      continue;

    Parse::Tree Tree;
    if (!parse(Tokens, Tree)) {
      return false;
    }
    if (BreakPoint == Parse)
      continue;
  }
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


bool Client::scan(std::istream &Stream, std::vector<Lex::Token> &Tokens) {
  if (Scanner->scan(Stream)) {
    Tokens = Scanner->getTokens();
    return true;
  }
  return false;
}

bool Client::parse(const std::vector<Lex::Token> &Tokens,
                   Parse::Tree &ParseTree) {
  if (Parser->parse(Tokens)) {
    ParseTree = Parser->buildTree();
    return true;
  }
  return false;
}
