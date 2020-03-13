#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(FileHeader &Header, class PackageTree &Tree)
    : Header(Header), Tree(Tree) {}

bool TypeLink::addSingleImport(const std::vector<std::string> &Name) {
  FileHeader *ImportHeader = Tree.findHeader(Name);
  if (ImportHeader == nullptr || ImportHeader->getName() == Header.getName()) {
    return false;
  }
  auto it = SingleImports.find(ImportHeader->getName());
  if (it != SingleImports.end()) {
    return (it->second->getASTNode() == ImportHeader->getASTNode());
  }
  SingleImports.emplace(ImportHeader->getName(), ImportHeader);
  return true;
}

void TypeLink::addDemandImport(const std::vector<std::string> &Name) {
  PackageNode *Node = Tree.findNode(Name);
  if (Node != nullptr) {
    OnDemandImports.emplace(Node);
  }
}

FileHeader *TypeLink::find(const std::vector<std::string> &Name) const {
  if (Name.size() == 0) {
    return nullptr;
  }
  // Name is fully qualified-name
  if (Name.size() > 1) {
    return Tree.findHeader(Name);
  }
  const std::string &SimpleName = Name.at(0);
  if (Header.getName() == SimpleName) {
    return &Header;
  }
  auto SingleImportsIt = SingleImports.find(SimpleName);
  if (SingleImportsIt != SingleImports.end()) {
    return SingleImportsIt->second;
  }
  FileHeader *SamePackageHeader = findSamePackage(SimpleName);
  if (SamePackageHeader != nullptr) {
    return SamePackageHeader;
  }
  FileHeader *DemandHeader = findDemand(SimpleName);
  if (DemandHeader != nullptr) {
    return DemandHeader;
  }
  return nullptr;
}

FileHeader *TypeLink::findSamePackage(const std::string &Name) const {
  PackageNode *Node = Tree.findNode(Header.getPackage());
  if (Node != nullptr) {
    return Node->findHeader(Name);
  }
  return nullptr;
}

FileHeader *TypeLink::findDemand(const std::string &Name) const {
  for (const auto &OnDemandImport : OnDemandImports) {
    FileHeader *Header = OnDemandImport->findHeader(Name);
    if (Header != nullptr) {
      return Header;
    }
  }
  return nullptr;
}

void TypeLinkVisitor::visit(const AST::Start &Start) {
  dispatchChildren(Start);
}

void TypeLinkVisitor::visit(const AST::SingleImportDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  SingleImports.emplace_back(Visitor.getName());
};

void TypeLinkVisitor::visit(const AST::DemandImportDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  DemandImports.emplace_back(Visitor.getName());
};

void TypeLinkVisitor::visit(const AST::InterfaceDeclaration &){};

void TypeLinkVisitor::visit(const AST::ClassDeclaration &){};

std::vector<std::vector<std::string>>
TypeLinkVisitor::getSingleImports() const {
  return std::move(SingleImports);
}

std::vector<std::vector<std::string>>
TypeLinkVisitor::getDemandImports() const {
  return std::move(DemandImports);
}

} // namespace Env
