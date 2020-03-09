#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <set>

#include "ASTNode.hpp"
#include "EnvScope.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"

class Client {
public:
  enum BreakPointType {
    VerifyName,
    Scan,
    Parse,
    Ast,
    Weed,
    FileHeader,
    Environment,
    TypeLink,
    Hierarchy,
  };
  bool outputToken = false;
  bool outputParse = false;
  bool outputAst = false;
  bool outputFileHeader = false;

  Client(Lex::Scanner *scanner = nullptr, Parse::DFA *parser = nullptr);
  void setBreakPoint(BreakPointType breakPoint);
  void addJavaFile(std::string &&files);
  void addJavaFiles(std::set<std::string> &&files);
  bool compile();

private:
  Lex::Scanner *scanner;
  Parse::DFA *parser;
  std::set<std::string> files{};
  BreakPointType breakPoint{Hierarchy};

  bool verifyFileName(std::string fileName);
  bool scan(std::istream &stream, std::vector<Lex::Token> &tokens);
  bool parse(const std::vector<Lex::Token> &tokens, Parse::Tree &parseTree);
  void buildAST(const Parse::Tree &ParseTree,
                std::unique_ptr<AST::Start> &ASTRoot);
  bool weed(const AST::Node &ast, const std::string &typeName);
};

#endif // CLIENT_HPP
