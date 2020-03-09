#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <set>

#include "ASTNode.hpp"
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
    Environment,
    TypeLink,
    Hierarchy,
  };
  bool outputToken = false;
  bool outputParse = false;
  bool outputAst = false;

  Client(Lex::Scanner *scanner = nullptr, Parse::DFA *parser = nullptr);
  void setBreakPoint(BreakPointType breakPoint);
  void addJavaFile(std::string &&files);
  void addJavaFiles(std::set<std::string> &&files);
  bool compile();

  bool verifyFileName(std::string fileName);
  std::optional<std::vector<Lex::Token>> scan(std::istream &stream);
  std::optional<Parse::Tree> parse(const std::vector<Lex::Token> &tokens);
  std::unique_ptr<AST::Start> buildAST(const Parse::Tree &ParseTree);
  std::unique_ptr<AST::Start> buildAST(std::istream &stream);
  bool weed(const AST::Node &ast, const std::string &typeName);

private:
  Lex::Scanner *scanner;
  Parse::DFA *parser;
  std::set<std::string> files{};
  BreakPointType breakPoint{Hierarchy};
};

#endif // CLIENT_HPP
