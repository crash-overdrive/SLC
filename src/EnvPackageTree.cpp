#include "EnvPackageTree.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

PackageNode::PackageNode(Type type, std::string name, Hierarchy *hierarchy)
    : type(type), name(std::move(name)), hierarchy(hierarchy) {}

PackageNode *PackageNode::update(Type type, const std::string &name,
                                 Hierarchy *header) {
  if (this->type != Global && this->type != Package)
    return nullptr;
  switch (type) {
  case Package:
    return updatePackage(type, name);
    break;
  case JoosType:
    return addType(type, name, header);
    break;
  case Global:
    break;
  }
  return nullptr;
}

PackageNode *PackageNode::find(const std::string &name) {
  auto it = children.find(name);
  return (it != children.end()) ? &it->second : nullptr;
}

/**
 * Find hierarchy under a package
 */
Hierarchy *PackageNode::findHierarchy(const std::string &name) {
  if (type == PackageNode::Global) {
    return nullptr;
  }
  PackageNode *node = find(name);
  if (node == nullptr) {
    return nullptr;
  }
  return node->hierarchy;
}

PackageNode *PackageNode::updatePackage(Type type, const std::string &name) {
  auto it = children.find(name);
  if (it != children.end()) {
    return (it->second.type == Package) ? &it->second : nullptr;
  }
  auto [childIt, flag] = children.emplace(name, PackageNode{type, name});
  return &childIt->second;
}

PackageNode *PackageNode::addType(Type type, const std::string &name,
                                  Hierarchy *header) {
  auto [it, flag] = children.emplace(name, PackageNode{type, name, header});
  return flag ? &it->second : nullptr;
}

Hierarchy *PackageTree::findType(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::JoosType) ? node->hierarchy : nullptr;
}

PackageNode *
PackageTree::findPackage(const std::vector<std::string> &path) const {
  PackageNode *node = findNode(path);
  if (node == nullptr) {
    return nullptr;
  }
  return (node->type == PackageNode::Package) ? node : nullptr;
}

bool PackageTree::update(std::vector<std::string> &&packagePath,
                         Hierarchy &hierarchy) {
  // No Package
  if (packagePath.size() == 0) {
    return true;
  }
  PackageNode *node = root.get();
  for (const auto &component : packagePath) {
    node = node->update(PackageNode::Package, component);
    if (!node) {
      return false;
    }
  }
  std::string identifier = hierarchy.getIdentifier();
  node = node->update(PackageNode::JoosType, identifier, &hierarchy);
  if (node == nullptr) {
    return false;
  }
  hierarchy.setPackage(std::move(packagePath));
  return true;
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
