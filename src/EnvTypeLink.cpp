#include "EnvTypeLink.hpp"

Env::TypeLink::TypeLink(std::unique_ptr<AST::Node> ASTRoot)
    : Target(std::move(ASTRoot)), Imports() {}

void Env::TypeLink::visit(TypeLinkVisitor &TypeLinkVisitor) {
  TypeLinkVisitor.setImports(Imports);
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

Env::TypeLinkVisitor::TypeLinkVisitor() : ErrorState(false) {}

void Env::TypeLinkVisitor::setPackageScope(Env::Scope &Root) {
  PackageScope = &Root;
}

void Env::TypeLinkVisitor::setImports(
    std::unordered_map<std::string, Scope &> &Imports) {
  this->Imports = &Imports;
}

bool Env::TypeLinkVisitor::error() { return ErrorState; }

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
    if (ErrorState)
      return;
  }
}

void Env::ScopeBuilder::visit(const AST::ClassDeclaration &Decl) {
  if (type == Scope::GLOBAL)
    return;
  type = Scope::CLASS;
  for (const auto &Child : Decl.getChildren()) {
    Child->accept(*this);
    if (ErrorState)
      return;
  }
}

void Env::ScopeBuilder::visit(const AST::InterfaceDeclaration &Decl) {
  if (type == Scope::GLOBAL)
    return;
  type = Scope::INTERFACE;
  for (const auto &Child : Decl.getChildren()) {
    Child->accept(*this);
    if (ErrorState)
      return;
  }
}

void Env::ScopeBuilder::visit(const AST::Identifier &Identifier) {
  PackageScope = PackageScope->update(Identifier.getName(), type);
  if (!PackageScope)
    ErrorState = true;
}

Env::ImportVisitor::ImportVisitor() : TypeLinkVisitor() {}

void Env::ImportVisitor::visit(const AST::Start &Start) {
  Current = PackageScope;
  for (const auto &Child : Start.getChildren()) {
    Child->accept(*this);
    if (ErrorState)
      return;
  }
}

void Env::ImportVisitor::visit(const AST::SingleImportDeclaration &Start) {
  for (const auto &Child : Start.getChildren()) {
    Child->accept(*this);
    if (ErrorState)
      return;
  }
  if (Current->getType() != Scope::CLASS && Current->getType() != Scope::INTERFACE) {
    ErrorState = true;
  }
  Current = PackageScope;
}

void Env::ImportVisitor::visit(const AST::DemandImportDeclaration &Decl) {
  for (const auto &Child : Decl.getChildren()) {
    Child->accept(*this);
    if (ErrorState)
      return;
  }
  if (Current->getType() != Scope::PACKAGE) {
    ErrorState = true;
  }
  Current = PackageScope;
}

void Env::ImportVisitor::visit(const AST::Identifier &Identifier) {
  Current = Current->find(Identifier.getName());
  if (Current == nullptr)
    ErrorState = true;
}
