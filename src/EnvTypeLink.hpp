#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"
#include "EnvPackageTree.hpp"
#include <unordered_set>

namespace Env {

class TypeLink {
public:
  TypeLink(FileHeader &Header, PackageTree &Tree);
  bool addSingleImport(const std::vector<std::string> &Name);
  void addDemandImport(const std::vector<std::string> &Name);
  FileHeader *find(const std::vector<std::string> &Name) const;

private:
  FileHeader &Header;
  PackageTree &Tree;
  std::unordered_map<std::string, FileHeader *> SingleImports;
  std::unordered_set<PackageNode *> OnDemandImports;
  FileHeader *findSamePackage(const std::string &Name) const;
  FileHeader *findDemand(const std::string &Name) const;
};

class TypeLinkVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &Start) override;
  void visit(const AST::SingleImportDeclaration &Decl) override;
  void visit(const AST::DemandImportDeclaration &Decl) override;
  void visit(const AST::InterfaceDeclaration &) override;
  void visit(const AST::ClassDeclaration &) override;
  std::vector<std::vector<std::string>> getSingleImports() const;
  std::vector<std::vector<std::string>> getDemandImports() const;

private:
  std::vector<std::vector<std::string>> SingleImports;
  std::vector<std::vector<std::string>> DemandImports;
};

} // namespace Env

#endif // ENVTYPELINK_HPP
