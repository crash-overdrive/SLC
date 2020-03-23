#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ASTNode.hpp"
#include "EnvJoosType.hpp"
#include "EnvPackageTree.hpp"
#include "EnvTypeLink.hpp"
#include "LexScanner.hpp"
#include "ParseDFA.hpp"

class Client {
public:
  enum BreakPointType {
    VerifyName,
    Scan,
    Parse,
    Ast,
    JoosType,
    Weed,
    LocalVariableAnalysis,
    PackageTree,
    TypeLink,
    Hierarchy,
    None,
  };

  Client(std::unique_ptr<Lex::Scanner> scanner,
         std::unique_ptr<Parse::DFA> parser);
  void setBreakPoint(BreakPointType breakPoint);
  void addPrintPoint(BreakPointType printPoint);

  bool compile(const std::vector<std::string> &fullNames);

  void buildJoosType(const std::string &fullName);
  void verifyFileName(const std::string &fullName);
  void openFile(const std::string &fullName);
  void scan(std::istream &stream, const std::string &fullName);
  void parse(const std::vector<Lex::Token> &tokens, const std::string &fulName);
  void buildAST(const Parse::Tree &parseTree, const std::string &fullName);
  void buildJoosType(std::unique_ptr<AST::Start> root,
                     const std::string &fulName);
  void weed(Env::JoosType joosType, const std::string &fullName);
  void localVariableAnalysis(Env::JoosType joosType,
                             const std::string &fulName);

  void buildEnvironment();
  void buildPackageTree();
  void buildTypeLink();
  void buildHierarchy();

  // buildAST is for debugging and testing
  std::unique_ptr<AST::Start> buildAST(const std::string &fullName);

private:
  struct Environment {
    Env::JoosType joosType;
    Env::TypeLink typeLink;
    explicit Environment(Env::JoosType joosType);
  };

  std::unique_ptr<Lex::Scanner> scanner;
  std::unique_ptr<Parse::DFA> parser;

  std::vector<Environment> environments;
  std::unique_ptr<AST::Start> logAstRoot;
  BreakPointType breakPoint{None};
  std::unordered_set<BreakPointType> printPoints;
  bool errorState{false};
};

#endif // CLIENT_HPP
