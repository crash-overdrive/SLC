#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "EnvHierarchy.hpp"
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
    TypeDeclaration,
    PackageTree,
    TypeLink,
    TypeBody,
    Weed,
    LocalVariableAnalysis,
    Hierarchy,
    None,
  };

  Client(std::unique_ptr<Lex::Scanner> scanner,
         std::unique_ptr<Parse::DFA> parser);
  void setBreakPoint(BreakPointType breakPoint);
  void addPrintPoint(BreakPointType printPoint);

  bool compile(std::vector<std::string> fullNames);

  void setupEnvironment(std::string fullName);
  void verifyFileName(std::string fullName);
  void openFile(std::string fullName);
  void scan(std::istream &stream, std::string fullName);
  void parse(const std::vector<Lex::Token> &tokens, std::string fullName);
  void buildAST(const Parse::Tree &parseTree, std::string fullName);
  void buildTypeDeclaration(std::unique_ptr<AST::Start> root,
                            std::string fullName);

  void buildEnvironment();
  void buildPackageTree();
  void buildTypeLink();
  void buildTypeBody();
  void weed();
  void localVariableAnalysis();
  void buildHierarchy();

  // buildAST is for debugging and testing
  std::unique_ptr<AST::Start> buildAST(std::string fullName);

private:
  struct Environment {
    Env::TypeDeclaration decl;
    Env::TypeLink typeLink;
    std::string fullName;
    Environment(Env::TypeDeclaration decl, std::string fileName);
  };

  bool buildClassHierarchy(Env::HierarchyGraph &graph,
                           Environment &environment);
  bool buildInterfaceHierarchy(Env::HierarchyGraph &graph,
                               Environment &environment);

  std::unique_ptr<Lex::Scanner> scanner;
  std::unique_ptr<Parse::DFA> parser;

  std::vector<Environment> environments;
  std::unique_ptr<AST::Start> logAstRoot;
  BreakPointType breakPoint{None};
  std::unordered_set<BreakPointType> printPoints;
  bool errorState{false};
};

#endif // CLIENT_HPP
