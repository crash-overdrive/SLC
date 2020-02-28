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
    Preprocess,
    Scan,
    Parse,
    Ast,
    Weed,
    Environment,
  };
  bool outputToken = false;
  bool outputParse = false;
  bool outputAst = false;

  Client(std::set<std::string> files);
  Client(Lex::Scanner &scanner, std::set<std::string> files);
  Client(Lex::Scanner &scanner, Parse::DFA &parser, std::set<std::string> files);
  void setBreakPoint(BreakPointType breakPoint);
  bool compile();

private:
  Lex::Scanner *scanner;
  Parse::DFA *parser;
  std::set<std::string> fileNames;
  BreakPointType breakPoint;

  bool scan(std::istream &stream, std::vector<Lex::Token> &tokens);
  bool parse(const std::vector<Lex::Token> &tokens, Parse::Tree &parseTree);
  void buildAST(const Parse::Tree &parseTree,
                std::unique_ptr<AST::Start> &astRoot);
  bool weed(const AST::Node &ast, const std::string &typeName);
  bool buildEnv(const std::vector<std::unique_ptr<AST::Start>> &astList,
                Env::Scope &packageScope);
};

#endif // CLIENT_HPP
