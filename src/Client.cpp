#include <fstream>

#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Client.hpp"

Client::Client(Lex::Scanner *scanner, Parse::DFA *parser)
    : scanner(scanner), parser(parser) {}

void Client::setBreakPoint(BreakPointType breakPoint) {
  this->breakPoint = breakPoint;
}

bool Client::compile() {
  std::vector<std::unique_ptr<AST::Start>> astList;

  for (const auto &file : files) {
    if (!verifyFileName(file)) {
      std::cerr << file << " is invalid " << std::endl;
      return false;
    }
    if (breakPoint == VerifyName) {
      continue;
    }

    std::ifstream javaFileStream;
    std::vector<Lex::Token> tokens;

    if (outputToken || outputParse || outputAst) {
      std::cout << file << std::endl;
    }

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
      for (auto const &token : tokens) {
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
    buildAST(Tree, astRoot);
    if (outputAst) {
      AST::Visitor Visitor = AST::Visitor();
      Visitor.setLog(std::cerr);
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
  if (breakPoint < Environment) {
    return true;
  }

  if (breakPoint == Environment) {
    return true;
  }
  return true;
}

void Client::addJavaFile(std::string &&file) { files.emplace(std::move(file)); }

void Client::addJavaFiles(std::set<std::string> &&files) {
  this->files = std::move(files);
}

bool Client::verifyFileName(std::string FileName) {
  const auto pos = FileName.find_last_of('/');
  if (pos != std::string::npos) {
    FileName = FileName.substr(pos+1);
  }
  const std::string Ext(".java");
  if (FileName.length() < Ext.length()) {
    return false;
  }
  size_t Position = FileName.find(".");
  return FileName.compare(Position, Ext.size(), Ext) == 0;
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
