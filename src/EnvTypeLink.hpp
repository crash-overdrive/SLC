#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTVisitor.hpp"
#include "EnvJoosType.hpp"
#include "EnvPackageTree.hpp"
#include <unordered_set>

namespace Env {

class TypeLink {
public:
  TypeLink(JoosType &joosType);
  void setPackage(std::vector<std::string> package);
  void setTree(std::shared_ptr<PackageTree> tree);
  bool addSingleImport(const std::vector<std::string> &name);
  bool addDemandImport(const std::vector<std::string> &name);
  JoosType *find(const std::vector<std::string> &name) const;

private:
  JoosType &joosType;
  std::vector<std::string> package;
  std::shared_ptr<PackageTree> tree;
  std::unordered_map<std::string, JoosType *> singleImports;
  std::unordered_set<PackageNode *> onDemandImports;
  JoosType *findSamePackage(const std::string &name) const;
  JoosType *findDemand(const std::string &name) const;
};

class TypeLinkVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::SingleImportDeclaration &decl) override;
  void visit(const AST::DemandImportDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &) override;
  void visit(const AST::ClassDeclaration &) override;
  std::vector<std::vector<std::string>> getSingleImports() const;
  std::vector<std::vector<std::string>> getDemandImports() const;

private:
  std::vector<std::vector<std::string>> singleImports;
  std::vector<std::vector<std::string>> demandImports;
};

} // namespace Env

#endif // ENVTYPELINK_HPP
