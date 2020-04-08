#include "ASTVisitorUtil.hpp"
#include "EnvTypeLink.hpp"

namespace AST {

void NameVisitor::visit(const AST::Name &name) { dispatchChildren(name); }

void NameVisitor::visit(const Identifier &identifier) {
  name.emplace_back(identifier.getName());
}

std::vector<std::string> NameVisitor::getName() { return std::move(name); }

void PropertiesVisitor::visit(const Modifier &modifier) {
  modifiers.emplace(Env::stringModifier.at(modifier.getName()));
}

void PropertiesVisitor::visit(const Identifier &node) {
  identifier = node.getName();
}

std::set<Env::Modifier> PropertiesVisitor::getModifiers() {
  return std::move(modifiers);
}

std::string PropertiesVisitor::getIdentifier() { return std::move(identifier); }

TypeVisitor::TypeVisitor(const Env::TypeLink &typeLink) : typeLink(typeLink) {}

void TypeVisitor::visit(const PrimitiveType &primitiveType) {
  type.keyword = Env::stringTypeKeyword.at(primitiveType.getType());
}

void TypeVisitor::visit(const SimpleType &simpleType) {
  type.keyword = Env::TypeKeyword::Simple;
  NameVisitor nameVisitor;
  nameVisitor.dispatchChildren(simpleType);
  type.declare = typeLink.find(nameVisitor.getName());
  if (!type.declare) {
    setError();
  }
}

void TypeVisitor::visit(const ArrayType &arrayType) {
  type.isArray = true;
  dispatchChildren(arrayType);
}

void TypeVisitor::visit(const AST::Name &name) {
  if (!type.isArray || type.keyword != Env::TypeKeyword::Void) {
    return;
  }
  NameVisitor nameVisitor;
  name.accept(nameVisitor);
  type.keyword = Env::TypeKeyword::Simple;
  type.declare = typeLink.find(nameVisitor.getName());
}

Env::Type TypeVisitor::getType() { return std::move(type); }

DeclarationVisitor::DeclarationVisitor(const Env::TypeLink &typeLink)
    : typeLink(typeLink) {}

void DeclarationVisitor::visit(const VariableDeclaration &node) {
  dispatchChildren(node);
}

void DeclarationVisitor::visit(const SingleVariableDeclaration &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  identifier = propertiesVisitor.getIdentifier();
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  if (typeVisitor.isErrorState()) {
    setError();
  }
  type = typeVisitor.getType();
}

Env::Type DeclarationVisitor::getType() { return std::move(type); }

std::string DeclarationVisitor::getIdentifier() {
  return std::move(identifier);
}

ArgumentsDeclarationVisitor::ArgumentsDeclarationVisitor(
    const Env::TypeLink &typeLink)
    : typeLink(typeLink) {}

void ArgumentsDeclarationVisitor::visit(const SingleVariableDeclaration &decl) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);
  if (typeVisitor.isErrorState()) {
    setError();
    return;
  }
  args.emplace_back(typeVisitor.getType());
}

std::vector<Env::Type> ArgumentsDeclarationVisitor::getArgs() {
  return std::move(args);
}

} // namespace AST
