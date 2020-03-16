#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ASTNode.hpp"
#include "EnvFileHeader.hpp"
#include "EnvPackageTree.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"

class Client {
public:
  enum BreakPointType {
    VerifyName,
    Scan,
    Parse,
    Ast,
    FileHeader,
    Weed,
    PackageTree,
    TypeLink,
    Hierarchy,
    None,
  };

  Client(std::unique_ptr<Lex::Scanner> scanner,
         std::unique_ptr<Parse::DFA> parser);
  void setBreakPoint(BreakPointType breakPoint);
  void addPrintPoint(BreakPointType printPoint);

  // buildAST is for debugging and testing
  std::unique_ptr<AST::Start> buildAST(const std::string &fullName);
  bool compile(const std::vector<std::string> &fullNames);

  void buildFileHeader(const std::string &fullName);
  void verifyFileName(const std::string &fullName);
  void openFile(const std::string &fullName);
  void scan(std::istream &stream, const std::string &fullName);
  void parse(const std::vector<Lex::Token> &tokens, const std::string &fulName);
  void buildAST(const Parse::Tree &parseTree, const std::string &fullName);
  void buildFileHeader(std::unique_ptr<AST::Start> root,
                       const std::string &fulName);
  void weed(Env::FileHeader fileHeader, const std::string &fullName);

  void buildEnvironment();
  void buildPackageTree();
  void buildTypeLink(const Env::PackageTree &tree);

private:
  std::unique_ptr<Lex::Scanner> scanner;
  std::unique_ptr<Parse::DFA> parser;

  std::unique_ptr<AST::Start> logAstRoot;
  std::vector<Env::Hierarchy> hierarchies;

  BreakPointType breakPoint{None};
  std::unordered_set<BreakPointType> printPoints;
  bool errorState{false};
};

#endif // CLIENT_HPP
