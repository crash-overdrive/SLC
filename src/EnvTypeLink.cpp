#include "EnvTypeLink.hpp"

Env::TypeLink::TypeLink(std::unique_ptr<AST::Node> ASTRoot)
    : Target(std::move(ASTRoot)), Imports() {}

void Env::TypeLink::visit(TypeLinkVisitor &TypeLinkVisitor) {
  //TypeLinkVisitor.setImports(Imports);
  Target->accept(TypeLinkVisitor);
}

Env::TypeLinkList::TypeLinkList()
    : Links(), PackageScope(std::make_unique<Scope>(Scope::GLOBAL)) {}

void Env::TypeLinkList::visit(TypeLinkVisitor &Visitor) {
  for (auto &Link : Links) {
    Visitor.setPackageScope(*PackageScope);
    Link.visit(Visitor);
  }
}

void Env::TypeLinkList::addAST(std::unique_ptr<AST::Node> ASTNode) {
  Links.emplace_back(std::move(ASTNode));
}

Env::TypeLinkVisitor::TypeLinkVisitor() : errorState(false) {}

void Env::TypeLinkVisitor::setPackageScope(Env::Scope &Root) {
  PackageScope = &Root;
}

void Env::TypeLinkVisitor::setImports(
    std::unordered_map<std::string, Scope &> &Imports) {
  this->Imports = &Imports;
}

bool Env::TypeLinkVisitor::error() { return errorState; }

Env::ScopeBuilder::ScopeBuilder() : TypeLinkVisitor(), type(Scope::GLOBAL) {}

void Env::ScopeBuilder::visit(const AST::Start &Start) {
  for (const auto &Child : Start.getChildren()) {
    Child->accept(*this);
  }
}

void Env::ScopeBuilder::visit(const AST::PackageDeclaration &Decl) {
  type = Scope::PACKAGE;
  for (const auto &Child : Decl.getChildren()) {
    Child->accept(*this);
    if (errorState)
      return;
  }
}

void Env::ScopeBuilder::visit(const AST::ClassDeclaration &Decl) {
  if (type == Scope::GLOBAL)
    return;
  type = Scope::CLASS;
  for (const auto &Child : Decl.getChildren()) {
    Child->accept(*this);
    if (errorState)
      return;
  }
}

void Env::ScopeBuilder::visit(const AST::Identifier &Identifier) {
  PackageScope = PackageScope->update(Identifier.getName(), type);
  if (!PackageScope)
    errorState = true;
}

Env::ImportVisitor::ImportVisitor() : TypeLinkVisitor() {}
