#include "NameResolver.hpp"
#include <iterator>

namespace Name {

void ResolverListener::listenLocal(off_t) {}
void ResolverListener::listenImplicit() {}
void ResolverListener::listenField(const Env::Field &) {}
void ResolverListener::listenStaticField(const Env::Field &) {}
void ResolverListener::listenMethod(const Env::Method &) {}
void ResolverListener::listenStaticMethod(const Env::Method &) {}
void ResolverListener::listenConstructor(const Env::Constructor &) {}
void ResolverListener::listenArrayLength() {}

FieldResolver::FieldResolver(const Env::Local &local,
                             const Env::TypeLink &typeLink,
                             ResolverListener &listener)
    : local(local), typeLink(typeLink), listener(listener) {}

std::optional<Env::Type>
FieldResolver::match(const std::vector<std::string> &name) {
  if (name.empty()) {
    type.reset();
    return type;
  }
  first = name.begin();
  last = name.end();
  matchLocal();
  return type;
}

std::optional<Env::Type> FieldResolver::match(Env::Type type,
                                              const std::string &identifier) {
  this->type.emplace(type);
  matchIdentifier(identifier);
  return this->type;
}

std::vector<std::string>::const_iterator FieldResolver::getFirst() const {
  return first;
}

void FieldResolver::matchLocal() {
  auto variable = local.findVariable(*first);
  if (variable) {
    type.emplace(variable->type);
    ++first;
    listener.listenLocal(variable->offset);
    matchName();
  } else {
    matchField();
  }
}

void FieldResolver::matchField() {
  const Env::TypeDeclaration &decl = typeLink.getDeclaration();
  const Env::Field *field = decl.contain.findField(*first);
  if (field && isInstanceVisible(typeLink, &decl, field)) {
    type.emplace(field->type);
    ++first;
    listener.listenImplicit();
    listener.listenField(*field);
    matchName();
  } else {
    matchStatic();
  }
}

void FieldResolver::matchStatic() {
  auto [it, matchDecl] = typeLink.match(first, last - 1);
  if (!matchDecl) {
    return;
  }
  const Env::Field *staticField = matchDecl->contain.findField(*it);
  if (staticField && isStaticVisible(typeLink, staticField)) {
    type.emplace(staticField->type);
    first = ++it;
    listener.listenStaticField(*staticField);
    matchName();
  }
}

void FieldResolver::matchIdentifier(const std::string &identifier) {
  if (type->isArray && identifier == "length") {
    type.emplace(Env::TypeKeyword::Integer);
    type->isFinal = true;
    listener.listenArrayLength();
    return;
  }
  if (type->isArray || type->keyword != Env::TypeKeyword::Simple) {
    type.reset();
    return;
  }
  const Env::TypeDeclaration *matchDecl = type->declare;
  if (!matchDecl) {
    type.reset();
    return;
  }
  const Env::Field *field = matchDecl->contain.findField(identifier);
  if (!field || !isInstanceVisible(typeLink, matchDecl, field)) {
    type.reset();
    return;
  }
  type.emplace(field->type);
  listener.listenField(*field);
}

void FieldResolver::matchName() {
  while (first != last) {
    matchIdentifier(*first);
    if (!type) {
      return;
    }
    ++first;
  }
}

MethodResolver::MethodResolver(const Env::Local &local,
                               const Env::TypeLink &typeLink,
                               ResolverListener &listener)
    : local(local), typeLink(typeLink), listener(listener) {}

std::optional<Env::Type>
MethodResolver::match(const std::vector<std::string> &name) {
  if (name.empty()) {
    return std::nullopt;
  }
  first = name.begin();
  last = name.end();
  matchImplicit();
  return type;
}

void MethodResolver::matchImplicit() {
  if (std::distance(first, last) == 1) {
    listener.listenImplicit();
    type.emplace(&typeLink.getDeclaration());
    matchIdentifier(*first);
  } else {
    matchField();
  }
}

void MethodResolver::matchField() {
  FieldResolver fieldResolver(local, typeLink, listener);
  std::vector<std::string> qualifiedName{first, last - 1};
  auto fieldType = fieldResolver.match(qualifiedName);
  if (fieldType) {
    type = fieldType;
    first = last - 1;
    matchIdentifier(*first);
    return;
  }
  if (qualifiedName.begin() == fieldResolver.getFirst()) {
    matchStaticMethod();
    return;
  }
  type.reset();
}

void MethodResolver::matchStaticMethod() {
  auto [it, matchDecl] = typeLink.match(first, last - 1);
  if (!matchDecl) {
    type.reset();
    return;
  }
  if (it == last - 1) {
    const Env::Method *staticMethod = matchDecl->contain.findMethod(*it, args);
    if (staticMethod && isStaticVisible(typeLink, staticMethod)) {
      type = staticMethod->returnType;
      first = last;
      listener.listenStaticMethod(*staticMethod);
      return;
    }
  }
  type.reset();
}

void MethodResolver::matchIdentifier(const std::string &identifier) {
  if (type->keyword != Env::TypeKeyword::Simple) {
    type.reset();
    return;
  }
  const Env::Method *method =
      type->declare->contain.findMethod(identifier, args);
  if (method && isInstanceVisible(typeLink, type->declare, method)) {
    type = method->returnType;
    listener.listenMethod(*method);
    return;
  }
  type.reset();
}

std::optional<Env::Type> MethodResolver::match(Env::Type type,
                                               const std::string &identifier) {
  this->type.emplace(type);
  matchIdentifier(identifier);
  return this->type;
}

void MethodResolver::setArgs(std::vector<Env::Type> args) {
  this->args = std::move(args);
}

ConstructorResolver::ConstructorResolver(const Env::TypeLink &typeLink,
                                         ResolverListener &listener)
    : typeLink(typeLink), listener(listener) {}

std::optional<Env::Type>
ConstructorResolver::match(Env::Type type, const std::vector<Env::Type> &args) {
  const Env::TypeDeclaration *typeDecl = type.declare;
  if (!typeDecl || typeDecl->modifiers.find(Env::Modifier::Abstract) !=
                       typeDecl->modifiers.end()) {
    return std::nullopt;
  }
  const Env::Constructor *constructor = typeDecl->body.findConstructors(args);
  if (!constructor) {
    return std::nullopt;
  }
  if (typeDecl != &typeLink.getDeclaration() &&
      constructor->modifiers.find(Env::Modifier::Protected) !=
          constructor->modifiers.end()) {
    return std::nullopt;
  }
  listener.listenConstructor(*constructor);
  return type;
}

ResolverFactory::ResolverFactory(const Env::Local &local,
                                 const Env::TypeLink &typeLink,
                                 ResolverListener &listener)
    : local(local), typeLink(typeLink), listener(listener) {}

FieldResolver ResolverFactory::getField() {
  return FieldResolver(local, typeLink, listener);
}

MethodResolver ResolverFactory::getMethod() {
  return MethodResolver(local, typeLink, listener);
}

ConstructorResolver ResolverFactory::getConstructor() {
  return ConstructorResolver(typeLink, listener);
}

void ResolverFactory::setListener(ResolverListener &listener) {
  this->listener = listener;
}

template <class T>
bool isInstanceVisible(const Env::TypeLink &typeLink,
                       const Env::TypeDeclaration *other, T t) {
  if (t->modifiers.find(Env::Modifier::Static) != t->modifiers.end()) {
    return false;
  }
  const Env::TypeDeclaration &decl = typeLink.getDeclaration();
  if (typeLink.belongSamePackage(t->declaration) ||
      (other->subType.find(&decl) != other->subType.end() &&
       decl.subType.find(t->declaration) != decl.subType.end())) {
    return true;
  }
  return t->modifiers.find(Env::Modifier::Protected) == t->modifiers.end();
}

template <class T> bool isStaticVisible(const Env::TypeLink &typeLink, T t) {
  if (t->modifiers.find(Env::Modifier::Static) == t->modifiers.end()) {
    return false;
  }
  const Env::TypeDeclaration &decl = typeLink.getDeclaration();
  if (typeLink.belongSamePackage(t->declaration) ||
      decl.subType.find(t->declaration) != decl.subType.end()) {
    return true;
  }
  return t->modifiers.find(Env::Modifier::Protected) == t->modifiers.end();
}

} // namespace Name
