#include "EnvPackageTree.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

PackageNode::PackageNode(Type type, const std::string &name, Hierarchy *hierarchy)
    : type(type), name(name), hierarchy(hierarchy) {}

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
 * Find header under a package
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
  auto It = children.find(name);
  if (It != children.end()) {
    return (It->second.type == Package) ? &It->second : nullptr;
  }
  auto [ChildIt, Flag] = children.emplace(name, PackageNode{type, name});
  return &ChildIt->second;
}

PackageNode *PackageNode::addType(Type type, const std::string &name,
                                  Hierarchy *header) {
  auto [It, Flag] = children.emplace(name, PackageNode{type, name, header});
  return Flag ? &It->second : nullptr;
}

Hierarchy *
PackageTree::findHierarchy(const std::vector<std::string> &Path) const {
  PackageNode *Node = findNode(Path);
  return (Node != nullptr) ? Node->hierarchy : nullptr;
}

PackageNode *PackageTree::findNode(const std::vector<std::string> &Path) const {
  PackageNode *Node = Root.get();
  for (const auto &Component : Path) {
    Node = Node->find(Component);
    if (!Node) {
      return nullptr;
    }
  }
  return Node;
}

bool PackageTree::update(std::vector<std::string> &&PackagePath,
                         Hierarchy &Hierarchy) {
  // No Package
  if (PackagePath.size() == 0) {
    return true;
  }
  PackageNode *Node = Root.get();
  for (const auto &Component : PackagePath) {
    Node = Node->update(PackageNode::Package, Component);
    if (!Node) {
      return false;
    }
  }
  std::string Identifier = Hierarchy.getIdentifier();
  Node = Node->update(PackageNode::JoosType, Identifier, &Hierarchy);
  if (Node == nullptr) {
    return false;
  }
  Hierarchy.setPackage(std::move(PackagePath));
  return true;
}

void PackageTreeVisitor::visit(const AST::Start &Start) {
  dispatchChildren(Start);
}

void PackageTreeVisitor::visit(const AST::PackageDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  packagePath = Visitor.getName();
}

std::vector<std::string> PackageTreeVisitor::getPackagePath() const {
  return std::move(packagePath);
}

} // namespace Env
