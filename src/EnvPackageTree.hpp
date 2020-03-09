#ifndef ENVSCOPE_HPP
#define ENVSCOPE_HPP

#include "ASTVisitor.hpp"
#include <map>
#include <string>

namespace Env {

class PackageNode {
public:
  enum Type { GLOBAL, PACKAGE, CLASS, INTERFACE };

  PackageNode(Type type, const std::string &name = "");
  PackageNode *update(Type type, const std::string &name);
  PackageNode *find(const std::string &name);

private:
  PackageNode *updatePackage(Type type, const std::string &name);
  PackageNode *addType(Type type, const std::string &name);
  Type type;
  std::string name;
  std::map<std::string, PackageNode> children{};
};

class PackageTree {
public:
  PackageTree(std::unique_ptr<PackageNode>);
  bool lookUp(const std::vector<std::string> &Name);
  bool update(PackageNode::Type type, const std::vector<std::string> &Name);

private:
  std::unique_ptr<PackageNode> Root;
};


class PackageTreeVisitor : public AST::TrackVisitor {
public:
  void visit(const AST::PackageDeclaration &Decl) override;
  void visit(const AST::ClassDeclaration &Decl) override;
  void visit(const AST::InterfaceDeclaration &Decl) override;
  void visit(const AST::Name &Name) override;

private:
  PackageNode::Type type;
  std::vector<std::string> fullyQualifiedName;
};

} // namespace Env

#endif // ENVSCOPE_HPP
