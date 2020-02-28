#include "Client.hpp"
#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Config.hpp"
#include <string>
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
  if (IncludeStdlib) {
    FileNames.insert(FileNames.end(), Stdlib2Files.begin(), Stdlib2Files.end());
  }
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
  Env::TypeLinkList Links;
  if (!buildEnv(ASTList, Links)) {
    std::cerr << "Failed to build environment\n";
    return false;
  }
  if (BreakPoint == Environment)
    return true;

  if (!typeLink(Links)) {
    std::cerr << "Failed to type link\n";
    return false;
  }
  if (BreakPoint == Environment)
    return true;
  return true;
}

bool Client::verifyFileName(const std::string &FileName) {
  std::string FileNameNoPath(FileName);
  const auto pos = FileNameNoPath.find_last_of('/');
  if (pos != std::string::npos) {
    FileNameNoPath = FileName.substr(pos+1);
  }
  const std::string Ext(".java");
  if (FileNameNoPath.length() < Ext.length()) {
    return false;
  }
  size_t Position = FileNameNoPath.find(".");
  return FileNameNoPath.compare(Position, Ext.size(), Ext) == 0;
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

bool Client::buildEnv(std::vector<std::unique_ptr<AST::Start>> &ASTList,
                      Env::TypeLinkList &Links) {
  for (auto &ASTRoot : ASTList) {
    Links.addAST(std::move(ASTRoot));
  }
  Env::ScopeBuilder Builder;
  Links.visit(Builder);
  return !Builder.error();
}

bool Client::typeLink(Env::TypeLinkList &Links) {
  Env::ImportVisitor Visitor;
  Links.visit(Visitor);
  return !Visitor.error();
}
