#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTVisitor.hpp"
#include "EnvPackageTree.hpp"

namespace Env {

class TypeLink {
public:
  TypeLink(TypeDeclaration &decl, std::shared_ptr<PackageTree> tree);
  bool setPackage(std::vector<std::string> package);
  bool addSingleImport(const std::vector<std::string> &name);
  bool addDemandImport(const std::vector<std::string> &name);
  TypeDeclaration *find(const std::vector<std::string> &name) const;
  TypeDeclaration *find(const std::string &name) const;
  template <class InputIt>
  std::pair<InputIt, TypeDeclaration *> find(InputIt first, InputIt last) const;
  TypeDeclaration *findSamePackage(const std::string &name) const;

private:
  TypeDeclaration &decl;
  std::vector<std::string> package;
  std::shared_ptr<PackageTree> tree;
  std::unordered_map<std::string, TypeDeclaration *> singleImports;
  std::unordered_set<PackageNode *> onDemandImports;
  TypeDeclaration *findDemand(const std::string &name) const;
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
