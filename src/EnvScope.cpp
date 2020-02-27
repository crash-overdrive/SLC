#include "EnvScope.hpp"

Env::Scope::Scope(Type type) : name(), type(type), children() {}

Env::Scope::Scope(const std::string &name, Type type)
    : name(name), type(type), children() {}

Env::Scope *Env::Scope::update(const std::string &name, Type type) {
  if (this->type != GLOBAL && this->type != PACKAGE)
    return nullptr;

  switch (type) {
  case PACKAGE:
    return updatePackage(name, type);
    break;
  case CLASS:
  case INTERFACE:
    return addType(name, type);
    break;
  case GLOBAL:
    break;
  }
  return nullptr;
}

Env::Scope *Env::Scope::find(const std::string &name) {
  auto it = children.find(name);
  return (it != children.end()) ? &it->second : nullptr;
}

Env::Scope *Env::Scope::updatePackage(const std::string &name, Type type) {
  auto It = children.find(name);
  if (It != children.end()) {
    if (It->second.type == PACKAGE) {
      return &It->second;
    } else {
      return nullptr;
    }
  }
  auto ChildIt = children.emplace(name, Scope{name, type});
  return &ChildIt.first->second;
}

Env::Scope *Env::Scope::addType(const std::string &name, Type type) {
  auto It = children.emplace(name, Scope{name, type});
  return It.second ? &It.first->second : nullptr;
}

const std::string &Env::Scope::getName() const { return name; }

Env::Scope::Type Env::Scope::getType() const { return type; }

void Env::ScopeBuilder::visit(const AST::Start &Start) {
  for (const auto &Child : Start.getChildren()) {
    Child->accept(*this);
  }
}

Env::ScopeBuilder::ScopeBuilder() : errorState(false), type(Scope::GLOBAL) {}

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
  Current = Current->update(Identifier.getName(), type);
  if (!Current)
    errorState = true;
}

void Env::ScopeBuilder::setRoot(Scope &Scope) { Current = &Scope; }

bool Env::ScopeBuilder::error() const { return errorState; }
