#include <fstream>

#include "Client.hpp"
#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"

Client::Client(Lex::Scanner &scanner, std::set<std::string> files)
  : scanner(&scanner), parser(nullptr), fileNames(files), breakPoint(Scan) {}

Client::Client(Lex::Scanner &scanner, Parse::DFA &parser, std::set<std::string> files)
  : scanner(&scanner), parser(&parser), fileNames(files), breakPoint(Environment) {}

void Client::setBreakPoint(BreakPointType breakPoint) {
  this->breakPoint = breakPoint;
}

bool Client::compile() {
  std::vector<std::unique_ptr<AST::Start>> astList;

  for (const auto &file : fileNames) {
    std::ifstream javaFileStream;
    std::vector<Lex::Token> tokens;

    javaFileStream.open(file);
    if (!javaFileStream.is_open()) {
      std::cerr << "Error: " << file << " could not be opened" << std::endl;
      return false;
    }
    if (!scan(javaFileStream, tokens)) {
      std::cerr << file << " is not recognized by scanner" << std::endl;
      return false;
    }
    if (outputToken) {
      for (auto const& token : tokens) {
        std::cout << token << std::endl;
      }
    }
    if (breakPoint == Scan) {
      continue;
    }

    Parse::Tree Tree;

    if (!parse(tokens, Tree)) {
      std::cerr << file << " is not recognized by parser\n";
      return false;
    }
    if (outputParse) {
      std::cout << Tree << std::endl;
    }
    if (breakPoint == Parse) {
      continue;
    }

    std::unique_ptr<AST::Start> astRoot = std::make_unique<AST::Start>();
    AST::PrintVisitor Visitor = AST::PrintVisitor(std::cout);

    buildAST(Tree, astRoot);
    if (outputAst) {
      astRoot->accept(Visitor);
    }
    astList.emplace_back(std::move(astRoot));
    if (breakPoint == Ast) {
      continue;
    }

    // TODO: implement weeder logic here
    if (breakPoint == Weed) {
      continue;
    }
  }

  if (breakPoint == Environment) {
     Env::Scope packageScope(Env::Scope::Type::GLOBAL);

    if (!buildEnv(astList, packageScope)) {
      std::cerr << "Failed to build environment\n";
      return false;
    }
    return true;
  }

  std::cerr << "Unrecognised breakpoint set: " << breakPoint << std::endl;
  return false;
}

bool Client::scan(std::istream &Stream, std::vector<Lex::Token> &Tokens) {
  if (scanner->scan(Stream)) {
    Tokens = scanner->getTokens();
    return true;
  }
  return false;
}

bool Client::parse(const std::vector<Lex::Token> &Tokens,
                   Parse::Tree &ParseTree) {
  if (parser->parse(Tokens)) {
    ParseTree = parser->buildTree();
    parser->clear();
    return true;
  }
  std::cerr << parser->buildTree();
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
