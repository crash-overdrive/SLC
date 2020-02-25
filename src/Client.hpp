#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ASTNode.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"

class Client {
public:
  enum BreakPointType {
    Preprocess,
    Scan,
    Parse,
    AST,
    Weed,
    Environment,
  };

  Client();
  Client(Lex::Scanner &Scanner);
  Client(Lex::Scanner &Scanner, Parse::DFA &Parser);
  void addJavaFile(const std::string &FileName);
  void addJavaFiles(std::initializer_list<std::string> List);
  void setStdlib(bool includeStdlib);
  void setBreakPoint(BreakPointType BP);
  bool process();

private:
  Lex::Scanner *Scanner;
  Parse::DFA *Parser;

  std::vector<std::string> FileNames;
  bool IncludeStdlib;
  BreakPointType BreakPoint;
  bool verifyFileName(const std::string &FileName);
  bool scan(std::istream &Stream, std::vector<Lex::Token> &Tokens);
  bool parse(const std::vector<Lex::Token> &Tokens, Parse::Tree &ParseTree);
  bool buildAST(const Parse::Tree &ParseTree, AST::Node &AST);
  bool weed(const AST::Node &AST, const std::string &TypeName);
};

#endif // CLIENT_HPP
