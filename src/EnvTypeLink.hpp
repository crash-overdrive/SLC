#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTVisitor.hpp"
#include "EnvHierarchy.hpp"
#include "EnvPackageTree.hpp"
#include <unordered_set>

namespace Env {

class TypeLink {
public:
  TypeLink(Hierarchy &hierarchy, PackageTree &tree);
  bool addSingleImport(const std::vector<std::string> &name);
  void addDemandImport(const std::vector<std::string> &name);
  Hierarchy *find(const std::vector<std::string> &name) const;

private:
  Hierarchy &hierarchy;
  PackageTree &tree;
  std::unordered_map<std::string, Hierarchy *> singleImports;
  std::unordered_set<PackageNode *> onDemandImports;
  Hierarchy *findSamePackage(const std::string &name) const;
  Hierarchy *findDemand(const std::string &name) const;
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
