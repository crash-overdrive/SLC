#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(Hierarchy &hierarchy, std::shared_ptr<PackageTree> tree)
    : hierarchy(hierarchy), tree(std::move(tree)) {}

bool TypeLink::addSingleImport(const std::vector<std::string> &name) {
  Hierarchy *importHierarchy = tree->findHierarchy(name);
  if (!importHierarchy ||
      importHierarchy->getIdentifier() == hierarchy.getIdentifier()) {
    return false;
  }
  auto it = singleImports.find(importHierarchy->getIdentifier());
  if (it != singleImports.end()) {
    return (it->second->getASTNode() == importHierarchy->getASTNode());
  }
  singleImports.emplace(importHierarchy->getIdentifier(), importHierarchy);
  return true;
}

bool TypeLink::addDemandImport(const std::vector<std::string> &name) {
  PackageNode *node = tree->findNode(name);
  if (!node) {
    return false;
  }
  onDemandImports.emplace(node);
  return true;
}

Hierarchy *TypeLink::find(const std::vector<std::string> &name) const {
  if (name.size() == 0) {
    return nullptr;
  }
  // Name is fully qualified-name
  if (name.size() > 1) {
    return tree->findHierarchy(name);
  }
  const std::string &simpleName = name.at(0);
  if (hierarchy.getIdentifier() == simpleName) {
    return &hierarchy;
  }
  auto singleImportsIt = singleImports.find(simpleName);
  if (singleImportsIt != singleImports.end()) {
    return singleImportsIt->second;
  }
  Hierarchy *samePackageHierarchy = findSamePackage(simpleName);
  if (samePackageHierarchy != nullptr) {
    return samePackageHierarchy;
  }
  Hierarchy *demandHierarchy = findDemand(simpleName);
  if (demandHierarchy != nullptr) {
    return demandHierarchy;
  }
  return nullptr;
}

Hierarchy *TypeLink::findSamePackage(const std::string &name) const {
  PackageNode *node = tree->findNode(hierarchy.getPackage());
  if (node != nullptr) {
    return node->findHierarchy(name);
  }
  return nullptr;
}

Hierarchy *TypeLink::findDemand(const std::string &name) const {
  for (const auto &onDemandImport : onDemandImports) {
    Hierarchy *hierarchy = onDemandImport->findHierarchy(name);
    if (hierarchy != nullptr) {
      return hierarchy;
    }
  }
  return nullptr;
}

void TypeLinkVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void TypeLinkVisitor::visit(const AST::SingleImportDeclaration &decl) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(decl);
  singleImports.emplace_back(visitor.getName());
};

void TypeLinkVisitor::visit(const AST::DemandImportDeclaration &decl) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(decl);
  demandImports.emplace_back(visitor.getName());
};

void TypeLinkVisitor::visit(const AST::InterfaceDeclaration &){};

void TypeLinkVisitor::visit(const AST::ClassDeclaration &){};

std::vector<std::vector<std::string>>
TypeLinkVisitor::getSingleImports() const {
  return std::move(singleImports);
}

std::vector<std::vector<std::string>>
TypeLinkVisitor::getDemandImports() const {
  return std::move(demandImports);
}

} // namespace Env
