#include "EnvPackageTree.hpp"

namespace Env {

PackageNode::PackageNode(Type type, const std::string &name)
    : type(type), name(name) {}

PackageNode *PackageNode::update(Type type, const std::string &name) {
  if (this->type != GLOBAL && this->type != PACKAGE)
    return nullptr;
  switch (type) {
  case PACKAGE:
    return updatePackage(type, name);
    break;
  case CLASS:
  case INTERFACE:
    return addType(type, name);
    break;
  case GLOBAL:
    break;
  }
  return nullptr;
}

PackageNode *PackageNode::find(const std::string &name) {
  auto it = children.find(name);
  return (it != children.end()) ? &it->second : nullptr;
}

PackageNode *PackageNode::updatePackage(Type type, const std::string &name) {
  auto It = children.find(name);
  if (It != children.end()) {
    return (It->second.type == PACKAGE) ? &It->second : nullptr;
  }
  auto ChildIt = children.emplace(name, PackageNode{type, name});
  return &ChildIt.first->second;
}

PackageNode *PackageNode::addType(Type type, const std::string &name) {
  auto It = children.emplace(name, PackageNode{type, name});
  return It.second ? &It.first->second : nullptr;
}

void PackageTreeVisitor::visit(const AST::PackageDeclaration &) {}

void PackageTreeVisitor::visit(const AST::Name &) {}

void PackageTreeVisitor::visit(const AST::ClassDeclaration &) {
  type = PackageNode::CLASS;
}

void PackageTreeVisitor::visit(const AST::InterfaceDeclaration &) {
  type = PackageNode::INTERFACE;
}

} // namespace Env
