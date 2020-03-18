#include "Client.hpp"
#include "ASTBuilder.hpp"
#include "ASTVisitor.hpp"
#include <fstream>
#include <iterator>

Client::Client(std::unique_ptr<Lex::Scanner> scanner,
               std::unique_ptr<Parse::DFA> parser)
    : scanner(std::move(scanner)), parser(std::move(parser)) {}

void Client::setBreakPoint(BreakPointType breakPoint) {
  this->breakPoint = breakPoint;
}

void Client::addPrintPoint(BreakPointType printPoint) {
  printPoints.emplace(printPoint);
}

std::unique_ptr<AST::Start> Client::buildAST(const std::string &fullName) {
  setBreakPoint(BreakPointType::Ast);
  buildHierarchy(fullName);
  return std::move(logAstRoot);
}

bool Client::compile(const std::vector<std::string> &fullNames) {
  for (const auto &file : fullNames) {
    buildHierarchy(file);
  }
  buildEnvironment();
  return !errorState;
}

void Client::buildHierarchy(const std::string &file) {
  if (errorState) {
    return;
  }
  verifyFileName(file);
}

void Client::verifyFileName(const std::string &fullName) {
  std::string fileName{fullName};
  const auto pos = fileName.find_last_of('/');
  if (pos != std::string::npos) {
    fileName = fileName.substr(pos + 1);
  }
  const std::string ext(".java");
  if (fileName.length() < ext.length()) {
    std::cerr << fileName << " is invalid\n";
    errorState = true;
    return;
  }
  size_t position = fileName.find(".");
  if (fileName.compare(position, ext.size(), ext) != 0) {
    std::cerr << fileName << " is invalid\n";
    errorState = true;
    return;
  }
  if (printPoints.size() > 0) {
    std::cerr << fullName << '\n';
  }
  if (breakPoint != VerifyName) {
    openFile(fullName);
  }
}

void Client::openFile(const std::string &fullName) {
  std::ifstream javaFileStream;
  javaFileStream.open(fullName);
  if (!javaFileStream.is_open()) {
    std::cerr << "Error: " << fullName << " could not be opened\n";
    errorState = true;
    return;
  }
  scan(javaFileStream, fullName);
}

void Client::scan(std::istream &stream, const std::string &fullName) {
  if (!scanner->scan(stream)) {
    std::cerr << fullName << " is not recognized by scanner\n";
    errorState = true;
    return;
  }
  std::vector<Lex::Token> tokens = scanner->getTokens();
  if (printPoints.find(Scan) != printPoints.end()) {
    std::copy(tokens.begin(), tokens.end(),
              std::ostream_iterator<Lex::Token>(std::cerr, "\n"));
  }
  if (breakPoint != Scan) {
    parse(tokens, fullName);
  }
}

void Client::parse(const std::vector<Lex::Token> &tokens,
                   const std::string &fullName) {
  if (!parser->parse(tokens)) {
    std::cerr << fullName << " is not recognized by parser\n";
    std::cerr << parser->buildTree() << '\n';
    errorState = true;
    return;
  }
  Parse::Tree tree = parser->buildTree();
  if (printPoints.find(Parse) != printPoints.end()) {
    std::cerr << tree << '\n';
  }
  if (breakPoint != Parse) {
    buildAST(tree, fullName);
  }
}

void Client::buildAST(const Parse::Tree &tree, const std::string &fullName) {
  auto root = std::make_unique<AST::Start>();
  const Parse::Node &parseRoot = tree.getRoot();
  AST::dispatch(parseRoot, *root);
  if (printPoints.find(Ast) != printPoints.end()) {
    AST::TrackVisitor visitor;
    visitor.setLog(std::cerr);
    root->accept(visitor);
  }
  if (breakPoint != Ast) {
    buildFileHeader(std::move(root), fullName);
  } else {
    logAstRoot = std::move(root);
  }
}

void Client::buildFileHeader(std::unique_ptr<AST::Start> node,
                             const std::string &fullName) {
  Env::JoosTypeVisitor typeVisitor;
  Env::JoosTypeBodyVisitor typeBodyVisitor;

  node->accept(typeVisitor);
  node->accept(typeBodyVisitor);
  Env::FileHeader fileHeader{typeVisitor.getModifiers(),
                             typeVisitor.getTypeDescriptor(), std::move(node)};
  for (auto const &field : typeBodyVisitor.getJoosFields()) {
    if (!fileHeader.addField(field)) {
      std::cerr << "Duplicate Field found in file\n" << field << '\n';
      std::cerr << "File header creation failed" << '\n';
      errorState = true;
      return;
    };
  }
  for (auto const &method : typeBodyVisitor.getJoosMethods()) {
    if (!fileHeader.addMethod(method)) {
      std::cerr << "Duplicate Method found in file\n" << method << '\n';
      std::cerr << "File header creation failed" << '\n';
      errorState = true;
      return;
    };
  }
  for (auto const &constructor : typeBodyVisitor.getJoosConstructors()) {
    if (!fileHeader.addConstructor(constructor)) {
      std::cerr << "Duplicate Constructor found in file\n"
                << constructor << '\n';
      std::cerr << "File header creation failed" << '\n';
      errorState = true;
      return;
    };
  }
  if (printPoints.find(FileHeader) != printPoints.end()) {
    std::cerr << fileHeader;
  }
  if (breakPoint != FileHeader) {
    weed(std::move(fileHeader), fullName);
  }
}

void Client::weed(Env::FileHeader fileHeader, const std::string &fullName) {
  (void)fullName;
  if (breakPoint != Weed) {
    buildHierarchy(std::move(fileHeader));
  }
}

void Client::buildHierarchy(Env::FileHeader fileHeader) {
  switch (fileHeader.getType()) {
  case Env::Type::Class:
    graph.addClass(std::move(fileHeader));
    break;
  case Env::Type::Interface: {
    graph.addInterface(std::move(fileHeader));
    break;
  }
  }
}

void Client::buildEnvironment() {
  if (errorState || breakPoint < PackageTree) {
    return;
  }
  buildPackageTree();
}

void Client::buildPackageTree() {
  auto tree = std::make_shared<Env::PackageTree>();
  for (auto &&hierarchy : graph.getHierarchies()) {
    Env::PackageTreeVisitor visitor;
    hierarchy->getASTNode()->accept(visitor);
    if (!tree->update(visitor.getPackagePath(), *hierarchy)) {
      std::cerr << "Error building package tree\n";
      errorState = true;
      return;
    };
  }
  if (breakPoint != PackageTree) {
    buildTypeLink(std::move(tree));
  }
}

void Client::buildTypeLink(std::shared_ptr<Env::PackageTree> tree) {
  for (auto &&hierarchy : graph.getHierarchies()) {
    Env::TypeLinkVisitor visitor;
    hierarchy->getASTNode()->accept(visitor);

    auto typeLink = std::make_unique<Env::TypeLink>(*hierarchy, tree);
    for (const auto &singleImport : visitor.getSingleImports()) {
      if (!typeLink->addSingleImport(singleImport)) {
        errorState = true;
        return;
      }
    }
    for (const auto &demandImport : visitor.getDemandImports()) {
      if (!typeLink->addDemandImport(demandImport)) {
        errorState = true;
        return;
      }
    }
    hierarchy->setTypeLink(std::move(typeLink));
  }
}
