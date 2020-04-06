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

Env::Type TypeVisitor::getType() { return std::move(type); }

ArgumentsVisitor::ArgumentsVisitor(const Env::TypeLink &typeLink)
    : typeLink(typeLink) {}

void ArgumentsVisitor::visit(const SingleVariableDeclaration &decl) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);
  if (typeVisitor.isErrorState()) {
    setError();
    return;
  }
  args.emplace_back(typeVisitor.getType());
}

std::vector<Env::Type> ArgumentsVisitor::getArgs() { return std::move(args); }

} // namespace AST
