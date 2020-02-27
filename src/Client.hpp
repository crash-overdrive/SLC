#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ASTNode.hpp"
#include "EnvScope.hpp"
#include "EnvTypeLink.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"

class Client {
public:
  enum BreakPointType {
    Preprocess,
    Scan,
    Parse,
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
  void buildAST(const Parse::Tree &ParseTree,
                std::unique_ptr<AST::Start> &ASTRoot);
  bool weed(const AST::Node &AST, const std::string &TypeName);
  bool buildEnv(std::vector<std::unique_ptr<AST::Start>> &ASTList,
      Env::TypeLinkList &Links);
  bool typeLink(Env::TypeLinkList &Links);
};

#endif // CLIENT_HPP
