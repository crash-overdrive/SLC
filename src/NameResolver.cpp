#include "NameResolver.hpp"
#include <iterator>

namespace Name {

Resolver::Resolver(const Env::Local &local, const Env::TypeLink &typeLink,
                   const Env::TypeDeclaration &decl)
    : local(local), typeLink(typeLink), decl(decl) {}

std::optional<Env::Type>
Resolver::findField(const std::vector<std::string> &name) const {
  if (name.empty()) {
    return std::nullopt;
  }
  auto objectType = matchObject(*name.begin());
  if (objectType) {
    return findField(*objectType, ++name.begin(), name.end());
  }

  // Match Static
  auto [it, matchDecl] = typeLink.find(name.begin(), name.end() - 1);
  if (!matchDecl) {
    return std::nullopt;
  }
  const Env::Field *staticMethod = matchDecl->contain.findField(*it);
  if (!staticMethod || !isVisible(matchDecl, staticMethod) ||
      !isStatic(staticMethod)) {
    return std::nullopt;
  }
  return findField(staticMethod->type, ++it, name.end());
}

std::optional<Env::Type>
Resolver::findField(Env::Type type, const std::string &identifier) const {
  if (type.isArray && identifier == "length") {
    return Env::TypeKeyword::Integer;
  }
  if (type.keyword != Env::TypeKeyword::Simple) {
    return std::nullopt;
  }
  const Env::TypeDeclaration *matchDecl = type.declare;
  const Env::Field *field = matchDecl->contain.findField(identifier);
  if (!field || !isVisible(matchDecl, field) || isStatic(field)) {
    return std::nullopt;
  }
  return field->type;
}

std::optional<Env::Type>
Resolver::findMethod(const std::vector<std::string> &name,
                     const std::vector<Env::Type> &args) const {

  if (name.empty()) {
    return std::nullopt;
  }
  if (name.size() == 1) {
    findMethod(Env::Type(&decl), args, name.begin(), name.end());
  }
  auto objectType = matchObject(*name.begin());
  if (objectType) {
    return findMethod(*objectType, args, ++name.begin(), name.end());
  }
  auto [it, matchDecl] = typeLink.find(name.begin(), name.end() - 1);
  if (!matchDecl) {
    return std::nullopt;
  }
  // Found type
  if (it == name.end() - 1) {
    const Env::Method *staticMethod = matchDecl->contain.findMethod(*it, args);
    if (!staticMethod || !isVisible(matchDecl, staticMethod) ||
        !isStatic(staticMethod)) {
      return std::nullopt;
    }
    return staticMethod->returnType;
  } else {
    const Env::Field *staticField = matchDecl->contain.findField(*it);
    if (!staticField || !isVisible(matchDecl, staticField) ||
        !isStatic(staticField)) {
      return std::nullopt;
    }
    return findMethod(staticField->type, args, ++it, name.end());
  }
}

std::optional<Env::Type>
Resolver::findMethod(Env::Type type, const std::string &identifier,
                     const std::vector<Env::Type> &args) const {
  if (type.keyword != Env::TypeKeyword::Simple) {
    return std::nullopt;
  }
  const Env::Method *method = decl.contain.findMethod(identifier, args);
  if (!method || !isVisible(type.declare, method) || isStatic(method)) {
    return std::nullopt;
  }
  return method->returnType;
}

std::optional<Env::Type>
Resolver::findConstructor(const std::vector<std::string> &name,
                          const std::vector<Env::Type> &args) const {
  Env::TypeDeclaration *foundDecl = typeLink.find(name);
  if (!foundDecl || foundDecl->modifiers.find(Env::Modifier::Abstract) !=
                        foundDecl->modifiers.end()) {
    return std::nullopt;
  }
  const Env::Constructor *constructor = foundDecl->body.findConstructors(args);
  if (!constructor || !isVisible(foundDecl, constructor)) {
    return std::nullopt;
  }
  return Env::Type{foundDecl};
}

template <class InputIt>
std::optional<Env::Type> Resolver::findField(Env::Type type, InputIt first,
                                             InputIt last) const {
  Env::Type resultType = type;
  for (auto it = first; it != last; ++it) {
    auto result = findField(resultType, *it);
    if (!result) {
      return std::nullopt;
    }
    resultType = *result;
  }
  return resultType;
}

template <class InputIt>
std::optional<Env::Type>
Resolver::findMethod(Env::Type type, const std::vector<Env::Type> &args,
                     InputIt first, InputIt last) const {
  auto result = findField(type, first, last - 1);
  if (!result) {
    return std::nullopt;
  }
  return findMethod(type, *(last - 1), args);
}

std::optional<Env::Type>
Resolver::matchObject(const std::string &identifier) const {
  auto variableType = local.findVariable(identifier);
  if (variableType) {
    return *variableType;
  }
  const Env::Field *field = decl.contain.findField(identifier);
  if (field && !isStatic(field)) {
    return field->type;
  }
  return std::nullopt;
}

template <class T>
bool Resolver::isVisible(const Env::TypeDeclaration *other, T t) const {
  if (typeLink.findSamePackage(other->identifier) ||
      decl.subType.find(other) != decl.subType.end()) {
    return true;
  }
  return t->modifiers.find(Env::Modifier::Protected) == t->modifiers.end();
}

template <class T> bool Resolver::isStatic(T t) const {
  return t->modifiers.find(Env::Modifier::Static) != t->modifiers.end();
}

} // namespace Name
