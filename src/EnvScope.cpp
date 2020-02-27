#include "EnvScope.hpp"

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
