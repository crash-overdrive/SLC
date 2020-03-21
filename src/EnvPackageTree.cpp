#include "EnvPackageTree.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

PackageNode::PackageNode(Type type, std::string name, JoosType *joosType)
    : type(type), name(std::move(name)), joosType(joosType) {}

PackageNode *PackageNode::update(Type type, const std::string &name,
                                 JoosType *header) {
  if (this->type != Type::Global && this->type != Type::Package)
    return nullptr;
  switch (type) {
  case Type::Package:
    return updatePackage(type, name);
    break;
  case Type::JoosType:
    return addType(type, name, header);
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
 * Find joosType under a package
 */
JoosType *PackageNode::findJoosType(const std::string &name) {
  if (type == PackageNode::Type::Global) {
    return nullptr;
  }
  PackageNode *node = find(name);
  if (node == nullptr) {
    return nullptr;
  }
  return node->joosType;
}

PackageNode *PackageNode::updatePackage(Type type, const std::string &name) {
  auto it = children.find(name);
  if (it != children.end()) {
    return (it->second.type == Type::Package) ? &it->second : nullptr;
  }
  auto [childIt, flag] = children.emplace(name, PackageNode{type, name});
  return &childIt->second;
}

PackageNode *PackageNode::addType(Type type, const std::string &name,
                                  JoosType *header) {
  auto [it, flag] = children.emplace(name, PackageNode{type, name, header});
  return flag ? &it->second : nullptr;
}

JoosType *PackageTree::findType(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::Type::JoosType) ? node->joosType : nullptr;
}

PackageNode *
PackageTree::findPackage(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::Type::Package) ? node : nullptr;
}

bool PackageTree::update(const std::vector<std::string> &packagePath,
                         JoosType &joosType) {
  // No Package
  if (packagePath.size() == 0) {
    return true;
  }
  PackageNode *node = root.get();
  for (const auto &component : packagePath) {
    node = node->update(PackageNode::Type::Package, component);
    if (!node) {
      return false;
    }
  }
  const std::string &identifier = joosType.identifier;
  node = node->update(PackageNode::Type::JoosType, identifier, &joosType);
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
