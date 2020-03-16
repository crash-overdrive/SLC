#ifndef ENVPACKAGETREE_HPP
#define ENVPACKAGETREE_HPP

#include "ASTVisitor.hpp"
#include "EnvHierarchy.hpp"
#include <map>
#include <string>

namespace Env {

class PackageNode {
public:
  enum Type { Global, Package, JoosType };

  PackageNode(Type type, const std::string &name = "",
              Hierarchy *hierarchy = nullptr);
  PackageNode *update(Type type, const std::string &name,
                      Hierarchy *hierarchy = nullptr);
  PackageNode *find(const std::string &name);
  Hierarchy *findHierarchy(const std::string &name);

private:
  friend class PackageTree;
  PackageNode *updatePackage(Type type, const std::string &name);
  PackageNode *addType(Type type, const std::string &name,
                       Hierarchy *hierarchy = nullptr);
  Type type;
  std::string name;
  Hierarchy *hierarchy;
  std::map<std::string, PackageNode> children{};
};

class PackageTree {
public:
  Hierarchy *findHierarchy(const std::vector<std::string> &path) const;
  PackageNode *findNode(const std::vector<std::string> &path) const;
  bool update(std::vector<std::string> &&packagePath,
              Hierarchy &hierarchy);

private:
  std::unique_ptr<PackageNode> root =
      std::make_unique<PackageNode>(PackageNode::Global);
};

class PackageTreeVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::PackageDeclaration &decl) override;
  std::vector<std::string> getPackagePath() const;

private:
  std::vector<std::string> packagePath;
};

} // namespace Env

#endif // ENVSCOPE_HPP
