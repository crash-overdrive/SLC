#include "Client.hpp"
#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"

#include <fstream>
#include <optional>

Client::Client(std::unique_ptr<Lex::Scanner> scanner,
               std::unique_ptr<Parse::DFA> parser)
    : scanner(std::move(scanner)), parser(std::move(parser)) {}

void Client::setBreakPoint(BreakPointType breakPoint) {
  this->breakPoint = breakPoint;
}

bool Client::compile() {
  std::vector<Env::FileHeader> fileHeaderList;

  for (const auto &file : files) {
    if (!verifyFileName(file)) {
      std::cerr << file << " is invalid " << std::endl;
      return false;
    }
    if (breakPoint == VerifyName) {
      continue;
    }

    if (outputToken || outputParse || outputAst || outputFileHeader) {
      std::cout << file << std::endl;
    }

    std::ifstream javaFileStream;

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
    if (breakPoint == Ast) {
      continue;
    }

    auto fileHeader = buildFileHeader(std::move(astRoot));
    if (!fileHeader) {
      std::cerr << "File header creation failed"
                << "\n";
      return false;
    }
    if (outputFileHeader) {
      std::cout << *fileHeader;
    }
    if (breakPoint == FileHeader) {
      continue;
    }
    fileHeaderList.emplace_back(std::move(*fileHeader));

    // TODO: implement weeder logic here
    // Weeder logic
    if (breakPoint == Weed) {
      continue;
    }
  }
  if (breakPoint < Weed) {
    return true;
  }

  auto Tree = buildPackageTree(fileHeaderList);
  if (!Tree) {
    std::cerr << "Error building package tree\n";
    return false;
  }
  if (breakPoint == PackageTree) {
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
    FileName = FileName.substr(pos + 1);
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

std::unique_ptr<AST::Start> Client::buildAST(const std::string &FileName) {
  std::ifstream JavaStream;
  JavaStream.open(FileName);
  return buildAST(*parse(*scan(JavaStream)));
}

std::optional<Env::FileHeader>
Client::buildFileHeader(std::unique_ptr<AST::Start> node) {
  std::optional<Env::FileHeader> fileHeader;
  Env::JoosTypeVisitor typeVisitor;
  Env::JoosTypeBodyVisitor typeBodyVisitor;

  node->accept(typeVisitor);
  node->accept(typeBodyVisitor);
  fileHeader =
      Env::FileHeader(typeVisitor.getModifiers(),
                      typeVisitor.getTypeDescriptor(), std::move(node));
  for (auto const &field : typeBodyVisitor.getJoosFields()) {
    if (!fileHeader->addField(field)) {
      std::cerr << "Duplicate Field found in file\n";
      std::cerr << field;
      return std::nullopt;
    };
  }
  for (auto const &method : typeBodyVisitor.getJoosMethods()) {
    if (!fileHeader->addMethod(method)) {
      std::cerr << "Duplicate Method found in file\n";
      std::cerr << method;
      return std::nullopt;
    };
  }
  for (auto const &constructor : typeBodyVisitor.getJoosConstructors()) {
    if (!fileHeader->addConstructor(constructor)) {
      std::cerr << "Duplicate Constructor found in file\n";
      std::cerr << constructor;
      return std::nullopt;
    };
  }
  return fileHeader;
}

std::optional<Env::PackageTree>
Client::buildPackageTree(std::vector<Env::FileHeader> &Headers) {
  std::optional<Env::PackageTree> Tree;
  for (auto &&Header : Headers) {
    Env::PackageTreeVisitor Visitor;
    Header.getASTNode()->accept(Visitor);
    if (!Tree->update(Visitor.getPackagePath(), Header)) {
      return std::nullopt;
    };
  }
  return Tree;
}
