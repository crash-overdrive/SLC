#ifndef ENVPACKAGETREE_HPP
#define ENVPACKAGETREE_HPP

#include "EnvTypeDeclaration.hpp"
#include <map>

namespace Env {

class PackageNode {
public:
  enum class Type { Global, Package, Declaration };

  PackageNode(Type type, std::string name = "",
              TypeDeclaration *decl = nullptr);
  PackageNode *update(Type type, const std::string &name,
                      TypeDeclaration *decl = nullptr);
  PackageNode *find(const std::string &name);
  TypeDeclaration *findDeclaration(const std::string &name);

private:
  friend class PackageTree;
  PackageNode *updatePackage(Type type, const std::string &name);
  PackageNode *addDeclaration(Type type, const std::string &name,
                              TypeDeclaration *decl = nullptr);
  Type type;
  std::string name;
  TypeDeclaration *decl;
  std::map<std::string, PackageNode> children;
};

class PackageTree {
public:
  TypeDeclaration *findDeclaration(const std::vector<std::string> &path) const;
  PackageNode *findPackage(const std::vector<std::string> &path) const;
  TypeDeclaration *findDefault(const std::string &name) const;
  bool update(const std::vector<std::string> &packagePath,
              TypeDeclaration &decl);

private:
  PackageNode *findNode(const std::vector<std::string> &path) const;
  std::unique_ptr<PackageNode> root =
      std::make_unique<PackageNode>(PackageNode::Type::Global);
  std::unordered_map<std::string, TypeDeclaration *> defaultPackage;
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

#endif // ENVPACKAGETREE_HPP
