#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"
#include "EnvPackageTree.hpp"

namespace Env {

class TypeLink {
public:
  TypeLink(FileHeader &Header, PackageTree &Tree);
  FileHeader *find(const std::vector<std::string> &Name);
  bool addSingleImport(const std::vector<std::string> &Name);
  bool addDemandImport(const std::vector<std::string> &Name);

private:
  FileHeader &Header;
  PackageTree &Tree;
  std::unordered_map<std::string, FileHeader &> SingleTypeImport;
  std::unordered_map<std::string, PackageNode &> OnDemandImport;
};

class TypeLinkVisitor : public AST::TrackVisitor {
public:
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
