#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(FileHeader &Header, PackageTree &Tree)
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
  if (Name.size() > 1) {
    return Tree.findHeader(Name);
  }
  const std::string &SimpleName = Name.at(0);
  if (Header.getName() == SimpleName) {
    return &Header;
  }
  auto it = SingleImports.find(SimpleName);
  if (it != SingleImports.end()) {
    return it->second;
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
