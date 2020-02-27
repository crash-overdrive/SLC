#include "Client.hpp"
#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include <fstream>

Client::Client()
    : Scanner(nullptr), Parser(nullptr), FileNames(), IncludeStdlib(true),
      BreakPoint(Preprocess) {}

Client::Client(Lex::Scanner &Scanner)
    : Scanner(&Scanner), Parser(nullptr), FileNames(), IncludeStdlib(true),
      BreakPoint(Scan) {}

Client::Client(Lex::Scanner &Scanner, Parse::DFA &Parser)
    : Scanner(&Scanner), Parser(&Parser), FileNames(), IncludeStdlib(true),
      BreakPoint(Environment) {}

void Client::addJavaFile(const std::string &FileName) {
  FileNames.emplace_back(FileName);
}

void Client::addJavaFiles(std::initializer_list<std::string> List) {
  FileNames.insert(FileNames.end(), List.begin(), List.end());
}

void Client::setBreakPoint(BreakPointType BreakPoint) {
  this->BreakPoint = BreakPoint;
}

void Client::setStdlib(bool IncludeStdlib) {
  this->IncludeStdlib = IncludeStdlib;
}

bool Client::process() {
  std::vector<std::unique_ptr<AST::Start>> ASTList;
  for (const auto &FileName : FileNames) {
    if (!verifyFileName(FileName)) {
      std::cerr << FileName << " has the wrong extension\n";
      return false;
    };
    if (BreakPoint == Preprocess) {
      std::cerr << FileName << " is valid format\n";
      continue;
    }

    std::ifstream JavaStream;
    JavaStream.open(FileName);
    if (!JavaStream.is_open()) {
      std::cerr << FileName << " could not be opened\n";
      return false;
    }

    std::vector<Lex::Token> Tokens;
    if (!scan(JavaStream, Tokens)) {
      std::cerr << FileName << " is not recognized by scanner\n";
      return false;
    }
    if (BreakPoint == Scan)
      continue;

    Parse::Tree Tree;
    if (!parse(Tokens, Tree)) {
      std::cerr << FileName << " is not recognized by parser\n";
      return false;
    }
    if (BreakPoint == Parse)
      continue;

    std::unique_ptr<AST::Start> ASTRoot = std::make_unique<AST::Start>();
    buildAST(Tree, ASTRoot);
    AST::PrintVisitor Visitor = AST::PrintVisitor(std::cerr);
    ASTRoot->accept(Visitor);
    ASTList.emplace_back(std::move(ASTRoot));
  }
  Env::Scope PackageScope(Env::Scope::Type::GLOBAL);
  if (!buildEnv(ASTList, PackageScope)) {
    std::cerr << "Failed to build environment\n";
    return false;
  }
  if (BreakPoint == Environment)
    return true;
  return true;
}

bool Client::verifyFileName(const std::string &FileName) {
  const std::string Ext(".java");
  if (FileName.length() < Ext.length()) {
    return false;
  }
  size_t Position = FileName.find(".");
  return FileName.compare(Position, Ext.size(), Ext) == 0;
}

bool Client::scan(std::istream &Stream, std::vector<Lex::Token> &Tokens) {
  if (Scanner->scan(Stream)) {
    Tokens = Scanner->getTokens();
    return true;
  }
  return false;
}

bool Client::parse(const std::vector<Lex::Token> &Tokens,
                   Parse::Tree &ParseTree) {
  if (Parser->parse(Tokens)) {
    ParseTree = Parser->buildTree();
    Parser->clear();
    return true;
  }
  std::cerr << Parser->buildTree();
  return false;
}

void Client::buildAST(const Parse::Tree &ParseTree,
                      std::unique_ptr<AST::Start> &ASTRoot) {
  const Parse::Node &ParseRoot = ParseTree.getRoot();
  dispatch(ParseRoot, *ASTRoot);
}

bool Client::buildEnv(const std::vector<std::unique_ptr<AST::Start>> &ASTList,
                      Env::Scope &PackageScope) {
  Env::ScopeBuilder Builder;
  for (const auto &ASTRoot : ASTList) {
    Builder.setRoot(PackageScope);
    ASTRoot->accept(Builder);
    if (Builder.error())
      return false;
  }
  return true;
}
