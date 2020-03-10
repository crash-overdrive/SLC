#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <set>

#include "ASTNode.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"
#include "EnvFileHeader.hpp"

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

  Client(std::unique_ptr<Lex::Scanner> scanner = nullptr,
         std::unique_ptr<Parse::DFA> parser = nullptr);
  void setBreakPoint(BreakPointType breakPoint);
  void addJavaFile(std::string &&files);
  void addJavaFiles(std::set<std::string> &&files);
  bool compile();

  bool verifyFileName(std::string fileName);
  std::optional<std::vector<Lex::Token>> scan(std::istream &stream);
  std::optional<Parse::Tree> parse(const std::vector<Lex::Token> &tokens);
  std::unique_ptr<AST::Start> buildAST(const Parse::Tree &ParseTree);
  std::unique_ptr<AST::Start> buildAST(const std::string &FileName);
  bool weed(const AST::Node &ast, const std::string &typeName);
  std::optional<Env::FileHeader> buildFileHeader(std::unique_ptr<AST::Start> &node);

private:
  std::unique_ptr<Lex::Scanner> scanner;
  std::unique_ptr<Parse::DFA> parser;
  std::set<std::string> files;
  BreakPointType breakPoint{Hierarchy};
};

#endif // CLIENT_HPP
