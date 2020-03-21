#ifndef ENVPACKAGETREE_HPP
#define ENVPACKAGETREE_HPP

#include "ASTVisitor.hpp"
#include "EnvJoosType.hpp"
#include <map>

namespace Env {

class PackageNode {
public:
  enum class Type { Global, Package, JoosType };

  PackageNode(Type type, std::string name = "", JoosType *joosType = nullptr);
  PackageNode *update(Type type, const std::string &name,
                      JoosType *joosType = nullptr);
  PackageNode *find(const std::string &name);
  JoosType *findJoosType(const std::string &name);

private:
  friend class PackageTree;
  PackageNode *updatePackage(Type type, const std::string &name);
  PackageNode *addType(Type type, const std::string &name,
                       JoosType *joosType = nullptr);
  Type type;
  std::string name;
  JoosType *joosType;
  std::map<std::string, PackageNode> children;
};

class PackageTree {
public:
  JoosType *findType(const std::vector<std::string> &path) const;
  PackageNode *findPackage(const std::vector<std::string> &path) const;
  bool update(const std::vector<std::string> &packagePath, JoosType &joosType);

private:
  PackageNode *findNode(const std::vector<std::string> &path) const;
  std::unique_ptr<PackageNode> root =
      std::make_unique<PackageNode>(PackageNode::Type::Global);
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
