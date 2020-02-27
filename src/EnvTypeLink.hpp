#ifndef ENVTYPELINK_HPP
#define ENVTYPELINK_HPP

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"
#include "EnvScope.hpp"
#include <string>
#include <unordered_map>

namespace Env {

class TypeLinkVisitor;

class TypeLink {
public:
  TypeLink(std::unique_ptr<AST::Node> ASTRoot);
  void link();
  void visit(TypeLinkVisitor &Visitor);

private:
  std::unique_ptr<AST::Node> Target;
  std::unordered_map<std::string, Scope &> Imports;
};

class TypeLinkList {
public:
  TypeLinkList();
  void visit(TypeLinkVisitor &ASTVisitor);
  void addAST(std::unique_ptr<AST::Node> Node);

private:
  std::vector<TypeLink> Links;
  std::unique_ptr<Env::Scope> PackageScope;
};

class TypeLinkVisitor : public AST::Visitor {
public:
  void setPackageScope(Env::Scope &Root);
  void setImports(std::unordered_map<std::string, Scope &> &Imports);
  bool error();

protected:
  TypeLinkVisitor();
  bool errorState;
  Env::Scope *PackageScope;
  std::unordered_map<std::string, Scope &> *Imports;
};

class ScopeBuilder : public TypeLinkVisitor {
public:
  ScopeBuilder();
  void visit(const AST::Start &Start) override;
  void visit(const AST::PackageDeclaration &Decl) override;
  void visit(const AST::ClassDeclaration &Decl) override;
  void visit(const AST::Identifier &Identifier) override;

private:
  Scope::Type type;
};

class ImportVisitor : public TypeLinkVisitor {
public:
  ImportVisitor();
  //void visit(const AST::Start &Start) override;
  //void visit(const AST::SingleImportDeclaration &Decl) override;
  //void visit(const AST::DemandImportDeclaration &Decl) override;
  //void visit(const AST::Identifier &Identifier) override;
};

} // namespace Env

#endif // ENVTYPELINK_HPP
