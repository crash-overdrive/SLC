#include "Client.hpp"
#include "ASMStructuralLib.hpp"
#include "ASTBuilder.hpp"
#include "CodeGenDeclaration.hpp"
#include "CodeGenLabel.hpp"
#include "CodeGenOffset.hpp"
#include "CodeGenStart.hpp"
#include "EnvLocal.hpp"
#include "TypeCheckerVisitor.hpp"
#include "Weeder.hpp"
#include <fstream>
#include <iterator>

template <class T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec) {
  stream << "[";
  for (auto const &element : vec) {
    stream << " " << element;
  }
  stream << "]";
  return stream;
}

Client::Client(std::unique_ptr<Lex::Scanner> scanner,
               std::unique_ptr<Parse::DFA> parser)
    : scanner(std::move(scanner)), parser(std::move(parser)) {}

void Client::setBreakPoint(BreakPointType breakPoint) {
  this->breakPoint = breakPoint;
}

void Client::addPrintPoint(BreakPointType printPoint) {
  printPoints.emplace(printPoint);
}

bool Client::compile(std::vector<std::string> fullNames) {
  environments.reserve(fullNames.size());
  for (auto &&fullName : fullNames) {
    setupEnvironment(std::move(fullName));
  }
  buildEnvironment();
  return !errorState;
}

void Client::setupEnvironment(std::string fullName) {
  if (errorState) {
    return;
  }
  verifyFileName(std::move(fullName));
}

void Client::verifyFileName(std::string fullName) {
  std::string fileName(fullName);
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
  if (position == std::string::npos) {
    std::cerr << fileName << " is invalid\n";
    errorState = true;
    return;
  }
  if (fileName.compare(position, ext.size(), ext) != 0) {
    std::cerr << fileName << " is invalid\n";
    errorState = true;
    return;
  }
  if (breakPoint != VerifyName) {
    openFile(std::move(fullName));
  }
}

void Client::openFile(std::string fullName) {
  std::ifstream javaFileStream;
  javaFileStream.open(fullName);
  if (!javaFileStream.is_open()) {
    std::cerr << "Error: " << fullName << " could not be opened\n";
    errorState = true;
    return;
  }
  scan(javaFileStream, std::move(fullName));
}

void Client::scan(std::istream &stream, std::string fullName) {
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
    parse(tokens, std::move(fullName));
  }
}

void Client::parse(const std::vector<Lex::Token> &tokens,
                   std::string fullName) {
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
    buildAST(tree, std::move(fullName));
  }
}

void Client::buildAST(const Parse::Tree &tree, std::string fullName) {
  auto root = std::make_unique<AST::Start>();
  const Parse::Node &parseRoot = tree.getRoot();
  AST::dispatch(parseRoot, *root);
  if (printPoints.find(Ast) != printPoints.end()) {
    AST::TrackVisitor visitor;
    visitor.setLog(std::cerr);
    root->accept(visitor);
  }
  if (breakPoint != Ast) {
    buildTypeDeclaration(std::move(root), std::move(fullName));
  } else {
    logAstRoot = std::move(root);
  }
}

void Client::buildTypeDeclaration(std::unique_ptr<AST::Start> node,
                                  std::string fullName) {
  Env::TypeDeclarationVisitor declVisitor;
  node->accept(declVisitor);
  Env::TypeDeclaration decl{
      declVisitor.getModifiers(),
      declVisitor.getKeyword(),
      declVisitor.getIdentifier(),
      std::move(node),
  };
  if (printPoints.find(TypeDeclaration) != printPoints.end()) {
    std::cerr << decl;
  }
  if (breakPoint != TypeDeclaration) {
    environments.emplace_back(std::move(decl), tree, std::move(fullName));
  }
}

void Client::buildEnvironment() {
  if (errorState || breakPoint < PackageTree) {
    return;
  }
  buildPackageTree();
}

void Client::buildPackageTree() {
  for (auto &environment : environments) {
    Env::PackageTreeVisitor visitor;
    environment.decl.astNode->accept(visitor);

    std::vector<std::string> packagePath = visitor.getPackagePath();
    if (!environment.typeLink.setPackage(std::move(packagePath))) {
      std::cerr << "Error building package tree\n";
      errorState = true;
      return;
    };
  }
  if (breakPoint != PackageTree) {
    buildTypeLink();
  }
}

void Client::buildTypeLink() {
  for (auto &environment : environments) {
    Env::TypeLinkVisitor visitor;
    environment.decl.astNode->accept(visitor);

    for (const auto &singleImport : visitor.getSingleImports()) {
      if (!environment.typeLink.addSingleImport(singleImport)) {
        std::cerr << "Error in Single Import\n";
        errorState = true;
        return;
      }
    }
    for (const auto &demandImport : visitor.getDemandImports()) {
      if (!environment.typeLink.addDemandImport(demandImport)) {
        std::cerr << "Error in on demand\n";
        errorState = true;
        return;
      }
    }
    environment.typeLink.addDemandImport({"java", "lang"});
  }
  if (breakPoint != TypeLink) {
    buildTypeBody();
  }
}

void Client::buildTypeBody() {
  for (auto &environment : environments) {
    Env::TypeBodyVisitor bodyVisitor(environment.typeLink);
    Env::TypeBody &body = environment.decl.body;
    environment.decl.astNode->accept(bodyVisitor);
    if (bodyVisitor.isErrorState()) {
      std::cerr << "Error while visiting body\n";
      errorState = true;
      return;
    }
    for (auto &&field : bodyVisitor.getFields()) {
      if (!body.addField(std::move(field))) {
        std::cerr << "Duplicate Field found in file\n";
        std::cerr << "Joos Type creation for file " << environment.fullName
                  << " failed\n";
        errorState = true;
        return;
      }
    }
    for (auto &&method : bodyVisitor.getMethods()) {
      if (!body.addMethod(std::move(method))) {
        std::cerr << "Duplicate Method found in file\n";
        std::cerr << "Joos Type creation for file " << environment.fullName
                  << " failed\n";
        errorState = true;
        return;
      }
    }
    for (auto &&constructor : bodyVisitor.getConstructors()) {
      if (!body.addConstructor(std::move(constructor))) {
        std::cerr << "Duplicate Constructor found in file\n";
        std::cerr << "Joos Type creation for file " << environment.fullName
                  << " failed\n";
        errorState = true;
        return;
      }
    }
  }
  if (breakPoint != TypeBody) {
    weed();
  }
}

void Client::weed() {
  for (auto &environment : environments) {
    Weed::Weeder weeder(environment.decl, environment.fullName);
    if (!weeder.verify()) {
      std::cerr << "Weeding failed for " << environment.fullName << '\n';
      errorState = true;
      return;
    }
  }
  if (breakPoint != Weed) {
    localVariableAnalysis();
  }
}

void Client::localVariableAnalysis() {
  bool log = (printPoints.find(LocalVariable) != printPoints.end());

  for (auto &environment : environments) {
    Env::TypeBody &body = environment.decl.body;
    for (auto const &constructor : body.getConstructors()) {
      Env::LocalTrackVisitor visitor(environment.typeLink, log);

      if (log) {
        std::cerr << "Local Variable Analysis for Constructor: "
                  << constructor.identifier
                  << " with args: " << constructor.args << " started...\n";
      }
      constructor.astNode->accept(visitor);
      if (log) {
        std::cerr << "Local Variable Analysis for Constructor: "
                  << constructor.identifier
                  << " with args: " << constructor.args << " ended...\n";
      }
      if (visitor.isErrorState()) {
        std::cerr << "Local Variable Analysis for file " << environment.fullName
                  << " failed" << '\n';
        errorState = true;
        return;
      }
    }
    for (auto const &method : body.getMethods()) {
      Env::LocalTrackVisitor visitor(environment.typeLink, log);

      if (log) {
        std::cerr << "Local Variable Analysis for Method: " << method.identifier
                  << " with args: " << method.args << " started...\n";
      }
      method.astNode->accept(visitor);
      if (log) {
        std::cerr << "Local Variable Analysis for Method: " << method.identifier
                  << " with args: " << method.args << " ended...\n";
      }
      if (visitor.isErrorState()) {
        std::cerr << "Local Variable Analysis for file " << environment.fullName
                  << " failed\n";
        errorState = true;
        return;
      }
    }
    body.setDeclaration(&environment.decl);
  }
  if (breakPoint != LocalVariable) {
    buildHierarchy();
  }
}

void Client::buildHierarchy() {
  Env::HierarchyGraph graph;
  for (auto &environment : environments) {
    bool flag;
    switch (environment.decl.keyword) {
    case Env::DeclarationKeyword::Class:
      flag = buildClassHierarchy(graph, environment);
      break;
    case Env::DeclarationKeyword::Interface:
      flag = buildInterfaceHierarchy(graph, environment);
      break;
    }
    if (!flag) {
      std::cerr << "Error building hierarchy\n";
      errorState = true;
      return;
    }
  }
  if (!graph.topologicalSort()) {
    std::cerr << "Error circular dependencies\n";
    errorState = true;
    return;
  }
  graph.buildSubType();
  if (!graph.buildContains()) {
    std::cerr << "Error building contains set\n";
    errorState = true;
    return;
  }
  if (breakPoint != Hierarchy) {
    typeCheck();
  }
}

bool Client::buildClassHierarchy(Env::HierarchyGraph &graph,
                                 Environment &environment) {
  Env::HierarchyVisitor visitor;
  environment.decl.astNode->accept(visitor);
  Env::ClassHierarchy classHierarchy(environment.decl);

  std::vector<std::string> super = visitor.getSuper();
  if (!super.empty()) {
    Env::TypeDeclaration *type = environment.typeLink.find(super);
    if (!type) {
      std::cout << super;
      std::cerr << "Extends from class not found in typelink\n";
      return false;
    }
    if (!classHierarchy.setExtends(type)) {
      std::cerr << "Extends for class can't be set\n";
      return false;
    }
  }
  for (auto &&interface : visitor.getInterfaces()) {
    Env::TypeDeclaration *type = environment.typeLink.find(interface);
    if (!type) {
      std::cerr << "Interface from class not found in typelink\n";
      return false;
    }
    if (!classHierarchy.addImplements(type)) {
      std::cerr << "Extends for class can't be set\n";
      return false;
    }
  }
  classHierarchy.setBaseObject(
      environment.typeLink.find({"java", "lang", "Object"}));
  graph.addClass(std::move(classHierarchy));
  return true;
}

bool Client::buildInterfaceHierarchy(Env::HierarchyGraph &graph,
                                     Environment &environment) {
  Env::HierarchyVisitor visitor;
  environment.decl.astNode->accept(visitor);
  Env::InterfaceHierarchy interfaceHierarchy(environment.decl);

  for (auto &&interface : visitor.getExtensions()) {
    Env::TypeDeclaration *type = environment.typeLink.find(interface);
    if (!type) {
      std::cerr << "Interface from interface not found in typelink\n";
      return false;
    }
    if (!interfaceHierarchy.addExtends(type)) {
      std::cerr << "Extends for interfaces can't be set\n";
      return false;
    }
  }
  Env::TypeDeclaration *base =
      environment.typeLink.find({"java", "lang", "Object"});
  if (!base) {
    std::cerr << "Missing Java Base Object\n";
    return false;
  }
  if (!interfaceHierarchy.setBaseObject(base)) {
    std::cerr << "Error setting base interface\n";
    return false;
  }
  graph.addInterface(std::move(interfaceHierarchy));
  return true;
}

void Client::typeCheck() {
  Type::Checker checker(*tree);
  for (auto &environment : environments) {
    if (environment.decl.keyword == Env::DeclarationKeyword::Interface) {
      continue;
    }
    Env::TypeBody &body{environment.decl.body};
    for (const auto &method : body.getMethods()) {
      Type::StatementVisitor visitor(environment.typeLink, *tree);
      Type::StaticListener listener;
      Type::StaticThisVisitor staticThisVisitor;
      if (method.modifiers.find(Env::Modifier::Static) !=
          method.modifiers.end()) {
        visitor.setListener(listener);
        method.astNode->accept(staticThisVisitor);
      }
      visitor.setReturnType(method.returnType);
      method.astNode->accept(visitor);
      if (visitor.isErrorState() || listener.isErrorState() ||
          staticThisVisitor.isErrorState()) {
        std::cerr << "Type Error in " << environment.fullName << '\n';
        std::cerr << method << '\n';
        errorState = true;
        return;
      }
    }
    for (const auto &constructor : body.getConstructors()) {
      Type::StatementVisitor visitor(environment.typeLink, *tree);
      constructor.astNode->accept(visitor);
      if (visitor.isErrorState()) {
        std::cerr << "Type Error in " << environment.fullName << '\n';
        std::cerr << constructor << '\n';
        errorState = true;
        return;
      }
    }

    std::unordered_set<std::string> fieldDeclared;
    for (const auto &field : body.getFields()) {
      Type::StatementVisitor typeVisitor(environment.typeLink, *tree);
      Type::StaticListener listener;
      Type::StaticThisVisitor staticThisVisitor;
      if (field.modifiers.find(Env::Modifier::Static) !=
          field.modifiers.end()) {
        typeVisitor.setListener(listener);
        field.astNode->accept(staticThisVisitor);
      }
      field.astNode->accept(typeVisitor);
      if (typeVisitor.isErrorState() || listener.isErrorState() ||
          staticThisVisitor.isErrorState()) {
        std::cerr << "Type Error in " << environment.fullName << '\n';
        std::cerr << field << '\n';
        errorState = true;
        return;
      }
      fieldDeclared.emplace(field.identifier);
      Type::FieldVisitor fieldVisitor(field.identifier, fieldDeclared,
                                      environment.decl.body);
      field.astNode->accept(fieldVisitor);
      if (fieldVisitor.isErrorState()) {
        std::cerr << "Field Error in " << environment.fullName << '\n';
        std::cerr << field << '\n';
        errorState = true;
        return;
      }
    }
  }
  if (breakPoint != TypeCheck) {
    codeGen();
  }
}

void Client::codeGen() {
  CodeGen::prepareOutput();

  std::streambuf *buf = nullptr;
  if (printPoints.find(CodeGen) != printPoints.end()) {
    buf = std::cerr.rdbuf();
  }
  codeGenOffset();
  codeGenLabel();
  codeGenFiles(buf);
  codeGenStart(buf);
}

void Client::codeGenOffset() {
  CodeGen::InterfaceOffsetGenerator interfaceGenerator;
  for (auto &environment : environments) {
    switch (environment.decl.keyword) {
    case Env::DeclarationKeyword::Interface:
      interfaceGenerator.update(environment.decl.body);
      break;
    case Env::DeclarationKeyword::Class:
      environment.decl.contain.updateOffset();
      break;
    }
  }
}

void Client::codeGenLabel() {
  for (auto &environment : environments) {
    if (environment.decl.keyword == Env::DeclarationKeyword::Class) {
      CodeGen::LabelGenerator labelGenerator(environment.typeLink.getPath());
      labelGenerator.generateDeclaration(environment.decl);
    }
  }
}

void Client::codeGenFiles(std::streambuf *log) const {
  std::streambuf *buf = log;

  for (const auto &environment : environments) {
    if (environment.decl.keyword != Env::DeclarationKeyword::Class) {
      continue;
    }
    std::ofstream ofstream;
    if (!log) {
      ofstream.open(CodeGen::getASMFile(environment.fullName));
      buf = ofstream.rdbuf();
    }
    std::ostream ostream(buf);
    if (log) {
      ostream << CodeGen::getASMFile(environment.fullName);
    }

    for (const auto &other : environments) {
      if (&other == &environment) {
        continue;
      }
      for (const auto &constructor : other.decl.body.getConstructors()) {
          if (!constructor.label.empty()) {
        ASM::printExtern(ostream, constructor.label);
          }
      }
      for (const auto &method : other.decl.body.getMethods()) {
        if (method.modifiers.find(Env::Modifier::Abstract) ==
            method.modifiers.end()) {
          if (!method.label.empty()) {
          ASM::printExtern(ostream, method.label);
          }
        }
      }
          if (!other.decl.contain.vtablelabel.empty()) {
      ASM::printExtern(ostream, other.decl.contain.vtablelabel);
          }
    }
  ASM::printExtern(ostream, "__malloc");

    CodeGen::DeclarationGenerator declGenerator(ostream, environment.typeLink);
    declGenerator.generateBody(environment.decl.body);
    declGenerator.generateContain(environment.decl.contain);
  }
}

void Client::codeGenStart(std::streambuf *log) const {
  std::streambuf *buf = log;
  std::ofstream ofstream;
  if (!log) {
    ofstream.open(CodeGen::outputStart);
    buf = ofstream.rdbuf();
  }
  std::ostream ostream(buf);
  if (log) {
    ostream << CodeGen::outputStart;
  }
  CodeGen::StartGenerator startGenerator(ostream);
  for (const auto &environment : environments) {
    startGenerator.generateStaticInit(environment.decl.body);
  }
  startGenerator.generateHeader();
  startGenerator.generateEntry();
}

std::unique_ptr<AST::Start> Client::buildAST(std::string fullName) {
  setBreakPoint(BreakPointType::Ast);
  setupEnvironment(std::move(fullName));
  return std::move(logAstRoot);
}

Client::Environment::Environment(Env::TypeDeclaration decl,
                                 std::shared_ptr<Env::PackageTree> tree,
                                 std::string fullName)
    : decl(std::move(decl)), typeLink(this->decl, std::move(tree)),
      fullName(std::move(fullName)) {}
