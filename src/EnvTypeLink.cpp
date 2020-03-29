#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(JoosType &joosType) : joosType(joosType) {}

void TypeLink::setPackage(std::vector<std::string> package) {
  this->package = std::move(package);
}

void TypeLink::setTree(std::shared_ptr<PackageTree> tree) {
  this->tree = std::move(tree);
}

bool TypeLink::addSingleImport(const std::vector<std::string> &name) {
  JoosType *importJoosType = tree->findType(name);
  if (!importJoosType) {
    return false;
  }
  // Self import
  if (importJoosType->astNode == joosType.astNode) {
    return true;
  }
  // Import name clash with self
  if (importJoosType->identifier == joosType.identifier) {
    return false;
  }
  auto it = singleImports.find(importJoosType->identifier);
  if (it != singleImports.end()) {
    return (it->second->astNode == importJoosType->astNode);
  }
  singleImports.emplace(importJoosType->identifier, importJoosType);
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

JoosType *TypeLink::find(const std::vector<std::string> &name) const {
  if (name.size() == 0) {
    return nullptr;
  }
  // Name is fully qualified-name
  if (name.size() > 1) {
    return tree->findType(name);
  }
  const std::string &simpleName = name.at(0);
  if (joosType.identifier == simpleName) {
    return &joosType;
  }
  auto singleImportsIt = singleImports.find(simpleName);
  if (singleImportsIt != singleImports.end()) {
    return singleImportsIt->second;
  }
  JoosType *samePackageJoosType = findSamePackage(simpleName);
  if (samePackageJoosType != nullptr) {
    return samePackageJoosType;
  }
  JoosType *demandJoosType = findDemand(simpleName);
  if (demandJoosType != nullptr) {
    return demandJoosType;
  }
  return nullptr;
}

JoosType *TypeLink::findSamePackage(const std::string &name) const {
  if (package.size() == 0) {
    return tree->findDefault(name);
  }
  PackageNode *node = tree->findPackage(package);
  if (node != nullptr) {
    return node->findJoosType(name);
  }
  return nullptr;
}

JoosType *TypeLink::findDemand(const std::string &name) const {
  JoosType *found = nullptr;
  for (const auto &onDemandImport : onDemandImports) {
    JoosType *joosType = onDemandImport->findJoosType(name);
    if (joosType == nullptr) {
      continue;
    }
    // Name Clash in demand
    if (found) {
      return nullptr;
    }
    found = joosType;
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
