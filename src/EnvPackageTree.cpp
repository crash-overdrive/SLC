#include "EnvPackageTree.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

PackageNode::PackageNode(Type type, const std::string &name, FileHeader *header)
    : type(type), name(name), header(header) {}

PackageNode *PackageNode::update(Type type, const std::string &name,
                                 FileHeader *header) {
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

PackageNode *PackageNode::updatePackage(Type type, const std::string &name) {
  auto It = children.find(name);
  if (It != children.end()) {
    return (It->second.type == Package) ? &It->second : nullptr;
  }
  auto ChildIt = children.emplace(name, PackageNode{type, name});
  return &ChildIt.first->second;
}

PackageNode *PackageNode::addType(Type type, const std::string &name,
                                  FileHeader *header) {
  auto It = children.emplace(name, PackageNode{type, name, header});
  return It.second ? &It.first->second : nullptr;
}

void PackageTreeVisitor::visit(const AST::PackageDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  packagePath = Visitor.getName();
}

void PackageTreeVisitor::visit(const AST::ClassDeclaration &) {}

void PackageTreeVisitor::visit(const AST::InterfaceDeclaration &) {}

std::vector<std::string> PackageTreeVisitor::getPackagePath() const {
  return packagePath;
}

FileHeader *PackageTree::lookUp(const std::vector<std::string> &PackagePath) {
  PackageNode *Node = Root.get();
  for (const auto &Component : PackagePath) {
    Node = Node->find(Component);
    if (!Node) {
      return nullptr;
    }
  }
  return Node->header;
}

bool PackageTree::update(const std::vector<std::string> &PackagePath,
                         FileHeader &Header) {
  PackageNode *Node = Root.get();
  for (const auto &Component : PackagePath) {
    Node = Node->update(PackageNode::Package, Component);
    if (!Node) {
      return false;
    }
  }
  Node = Node->update(PackageNode::JoosType, Header.getName(), &Header);
  return Node != nullptr;
}

} // namespace Env
