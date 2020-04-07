#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeLink::TypeLink(TypeDeclaration &decl, std::shared_ptr<PackageTree> tree)
    : decl(decl), tree(std::move(tree)) {}

const TypeDeclaration &TypeLink::getDeclaration() const { return decl; }

bool TypeLink::setPackage(std::vector<std::string> package) {
  this->package = std::move(package);
  return tree->update(this->package, decl);
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
  if (name.size() == 1) {
    return find(name.at(0));
  }
  // Name is fully qualified-name
  if (name.size() > 1) {
    // Make sure simple name does not resolve to any type
    if (find(name.at(0))) {
      return nullptr;
    }
    return tree->findDeclaration(name);
  }
  return nullptr;
}

TypeDeclaration *TypeLink::find(const std::string &name) const {
  if (decl.identifier == name) {
    return &decl;
  }
  auto singleImportsIt = singleImports.find(name);
  if (singleImportsIt != singleImports.end()) {
    return singleImportsIt->second;
  }
  TypeDeclaration *samePackageDecl = findPackage(name);
  if (samePackageDecl != nullptr) {
    return samePackageDecl;
  }
  TypeDeclaration *demandDecl = findDemand(name);
  if (demandDecl != nullptr) {
    return demandDecl;
  }
  return nullptr;
}

template <typename InputIt>
std::pair<InputIt, TypeDeclaration *> TypeLink::find(InputIt first,
                                                     InputIt last) const {
  if (first == last) {
    return std::make_pair(last, nullptr);
  }
  TypeDeclaration *decl = find(*first);
  if (decl) {
    return std::make_pair(++first, decl);
  }
  PackageNode *node = tree->getRoot();
  for (auto it = first; it != last; ++it) {
    node = node->find(*it);
    if (!node) {
      return std::make_pair(++it, nullptr);
    }
    TypeDeclaration *nodeDecl = node->getDeclaration();
    if (nodeDecl) {
      return std::make_pair(++it, nodeDecl);
    }
  }
  return std::make_pair(last, nullptr);
}

template std::pair<std::vector<std::string>::const_iterator, TypeDeclaration *>
TypeLink::find<std::vector<std::string>::const_iterator>(
    std::vector<std::string>::const_iterator first,
    std::vector<std::string>::const_iterator last) const;

bool TypeLink::belongSamePackage(const TypeDeclaration *decl) const {
  return decl == findPackage(decl->identifier);
}

TypeDeclaration *TypeLink::findPackage(const std::string &name) const {
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
