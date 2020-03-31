#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(TypeDeclaration &decl) : decl(decl) {}

void TypeLink::setPackage(std::vector<std::string> package) {
  this->package = std::move(package);
}

void TypeLink::setTree(std::shared_ptr<PackageTree> tree) {
  this->tree = std::move(tree);
}

bool TypeLink::addSingleImport(const std::vector<std::string> &name) {
  TypeDeclaration *importDecl = tree->findDeclaration(name);
  if (!importDecl) {
    return false;
  }
  // Self import
  if (importDecl->astNode == decl.astNode) {
    return true;
  }
  // Import name clash with self
  if (importDecl->identifier == decl.identifier) {
    return false;
  }
  auto it = singleImports.find(importDecl->identifier);
  if (it != singleImports.end()) {
    return (it->second->astNode == importDecl->astNode);
  }
  singleImports.emplace(importDecl->identifier, importDecl);
  return true;
}

bool TypeLink::addDemandImport(const std::vector<std::string> &name) {
  PackageNode *node = tree->findPackage(name);
  if (!node) {
    return false;
  }
  onDemandImports.emplace(node);
  return true;
}

TypeDeclaration *TypeLink::find(const std::vector<std::string> &name) const {
  if (name.size() == 0) {
    return nullptr;
  }
  // Name is fully qualified-name
  if (name.size() > 1) {
    // Make sure simple name does not resolve to any type
    if (find({name.at(0)})) {
      return nullptr;
    }
    return tree->findDeclaration(name);
  }
  const std::string &simpleName = name.at(0);
  if (decl.identifier == simpleName) {
    return &decl;
  }
  auto singleImportsIt = singleImports.find(simpleName);
  if (singleImportsIt != singleImports.end()) {
    return singleImportsIt->second;
  }
  TypeDeclaration *samePackageDecl = findSamePackage(simpleName);
  if (samePackageDecl != nullptr) {
    return samePackageDecl;
  }
  TypeDeclaration *demandDecl = findDemand(simpleName);
  if (demandDecl != nullptr) {
    return demandDecl;
  }
  return nullptr;
}

TypeDeclaration *TypeLink::findSamePackage(const std::string &name) const {
  if (package.size() == 0) {
    return tree->findDefault(name);
  }
  PackageNode *node = tree->findPackage(package);
  if (node != nullptr) {
    return node->findDeclaration(name);
  }
  return nullptr;
}

TypeDeclaration *TypeLink::findDemand(const std::string &name) const {
  TypeDeclaration *found = nullptr;
  for (const auto &onDemandImport : onDemandImports) {
    TypeDeclaration *decl = onDemandImport->findDeclaration(name);
    if (decl == nullptr) {
      continue;
    }
    // Name Clash in demand
    if (found) {
      return nullptr;
    }
    found = decl;
  }
  return found;
}

void TypeLinkVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void TypeLinkVisitor::visit(const AST::SingleImportDeclaration &decl) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(decl);
  singleImports.emplace_back(visitor.getName());
}

void TypeLinkVisitor::visit(const AST::DemandImportDeclaration &decl) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(decl);
  demandImports.emplace_back(visitor.getName());
}

void TypeLinkVisitor::visit(const AST::InterfaceDeclaration &) {}

void TypeLinkVisitor::visit(const AST::ClassDeclaration &) {}

std::vector<std::vector<std::string>>
TypeLinkVisitor::getSingleImports() const {
  return std::move(singleImports);
}

std::vector<std::vector<std::string>>
TypeLinkVisitor::getDemandImports() const {
  return std::move(demandImports);
}

} // namespace Env
