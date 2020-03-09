#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include "Client.hpp"

#include <fstream>
#include <optional>

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

    if (outputToken || outputParse || outputAst) {
      std::cout << file << std::endl;
    }

    javaFileStream.open(file);
    if (!javaFileStream.is_open()) {
      std::cerr << "Error: " << file << " could not be opened" << std::endl;
      return false;
    }

    auto tokens = scan(javaFileStream);
    if (!tokens) {
      std::cerr << file << " is not recognized by scanner" << std::endl;
      return false;
    }
    if (outputToken) {
      for (auto const &token : *tokens) {
        std::cout << token << std::endl;
      }
    }
    if (breakPoint == Scan) {
      continue;
    }

    auto Tree = parse(*tokens);
    if (!Tree) {
      std::cerr << file << " is not recognized by parser\n";
      return false;
    }
    if (outputParse) {
      std::cout << *Tree << std::endl;
    }
    if (breakPoint == Parse) {
      continue;
    }

    std::unique_ptr<AST::Start> astRoot = buildAST(*Tree);
    if (outputAst) {
      AST::TrackVisitor Visitor;
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


std::optional<std::vector<Lex::Token>> Client::scan(std::istream &Stream) {
  if (scanner->scan(Stream)) {
    return scanner->getTokens();
  }
  return std::nullopt;
}

std::optional<Parse::Tree>
Client::parse(const std::vector<Lex::Token> &Tokens) {
  if (parser->parse(Tokens)) {
    return parser->buildTree();
  }
  return std::nullopt;
}

std::unique_ptr<AST::Start> Client::buildAST(const Parse::Tree &ParseTree) {
  std::unique_ptr<AST::Start> Root = std::make_unique<AST::Start>();
  const Parse::Node &ParseRoot = ParseTree.getRoot();
  dispatch(ParseRoot, *Root);
  return Root;
}

std::unique_ptr<AST::Start> Client::buildAST(std::istream &stream) {
  return buildAST(*parse(*scan(stream)));
}
