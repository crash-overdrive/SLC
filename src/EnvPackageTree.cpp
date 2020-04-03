#include "EnvPackageTree.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

PackageNode::PackageNode(Type type, std::string name, TypeDeclaration *decl)
    : type(type), name(std::move(name)), decl(decl) {}

PackageNode *PackageNode::update(Type type, const std::string &name,
                                 TypeDeclaration *decl) {
  if (this->type != Type::Global && this->type != Type::Package)
    return nullptr;
  switch (type) {
  case Type::Package:
    return updatePackage(type, name);
    break;
  case Type::Declaration:
    return addDeclaration(type, name, decl);
    break;
  case Type::Global:
    break;
  }
  return nullptr;
}

PackageNode *PackageNode::find(const std::string &name) {
  auto it = children.find(name);
  return (it != children.end()) ? &it->second : nullptr;
}

/**
 * Find declaration under a package
 */
TypeDeclaration *PackageNode::findDeclaration(const std::string &name) {
  if (type == PackageNode::Type::Global) {
    return nullptr;
  }
  PackageNode *node = find(name);
  if (node == nullptr) {
    return nullptr;
  }
  return node->decl;
}

TypeDeclaration *PackageNode::getDeclaration() const { return decl; }

PackageNode *PackageNode::updatePackage(Type type, const std::string &name) {
  auto it = children.find(name);
  if (it != children.end()) {
    return (it->second.type == Type::Package) ? &it->second : nullptr;
  }
  auto [ChildIt, Flag] = children.emplace(name, PackageNode{type, name});
  return &ChildIt->second;
}

PackageNode *PackageNode::addDeclaration(Type type, const std::string &name,
                                         TypeDeclaration *decl) {
  auto [it, flag] = children.emplace(name, PackageNode{type, name, decl});
  return flag ? &it->second : nullptr;
}

TypeDeclaration *
PackageTree::findDeclaration(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::Type::Declaration) ? node->decl : nullptr;
}

PackageNode *
PackageTree::findPackage(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::Type::Package) ? node : nullptr;
}

PackageNode *PackageTree::getRoot() const { return root.get(); }

TypeDeclaration *PackageTree::findDefault(const std::string &name) const {
  auto it = defaultPackage.find(name);
  if (it == defaultPackage.end()) {
    return nullptr;
  }
  return it->second;
}

bool PackageTree::update(const std::vector<std::string> &packagePath,
                         TypeDeclaration &decl) {
  // Default Package
  if (packagePath.size() == 0) {
    auto [it, flag] = defaultPackage.emplace(decl.identifier, &decl);
    return flag;
  }
  PackageNode *node = root.get();
  for (const auto &component : packagePath) {
    node = node->update(PackageNode::Type::Package, component);
    if (!node) {
      return false;
    }
  }
  const std::string &identifier = decl.identifier;
  node = node->update(PackageNode::Type::Declaration, identifier, &decl);
  return node != nullptr;
}

PackageNode *PackageTree::findNode(const std::vector<std::string> &path) const {
  PackageNode *node = root.get();
  for (const auto &component : path) {
    node = node->find(component);
    if (!node) {
      return nullptr;
    }
  }
  return node;
}

void PackageTreeVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void PackageTreeVisitor::visit(const AST::PackageDeclaration &decl) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(decl);
  packagePath = visitor.getName();
}

std::vector<std::string> PackageTreeVisitor::getPackagePath() const {
  return std::move(packagePath);
}

} // namespace Env
