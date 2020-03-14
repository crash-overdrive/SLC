#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <optional>
#include <set>

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

  bool compile(const std::vector<std::string> &fileNames);
  void buildFileHeader(const std::string &fileName);
  void verifyFileName(const std::string &fullName);
  void openFile(const std::string &fullName);
  void scan(std::istream &stream, const std::string &fullName);
  void parse(const std::vector<Lex::Token> &tokens, const std::string &fulName);
  void buildAST(const Parse::Tree &ParseTree, const std::string &fullName);
  void buildFileHeader(std::unique_ptr<AST::Start> Root,
                       const std::string &fulName);
  void weed(Env::FileHeader fileHeader, const std::string &fullName);

  void buildEnvironment();
  void buildPackageTree();
  void buildTypeLink(const Env::PackageTree &Tree);

private:
  std::unique_ptr<Lex::Scanner> scanner;
  std::unique_ptr<Parse::DFA> parser;
  std::vector<Env::FileHeader> fileHeaders;

  BreakPointType breakPoint{None};
  std::unordered_set<BreakPointType> printPoints;
  bool errorState{false};
};

#endif // CLIENT_HPP

