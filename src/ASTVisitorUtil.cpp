#include "ASTVisitorUtil.hpp"

namespace AST {

void NameVisitor::visit(const AST::Name &name) {
  dispatchChildren(name);
}

void NameVisitor::visit(const Identifier &identifier) {
  name.emplace_back(identifier.getName());
}

std::vector<std::string> NameVisitor::getName() {
  return std::move(name);
}

void PropertiesVisitor::visit(const Modifier &modifier) {
  modifiers.emplace(modifier.getCode());
}

void PropertiesVisitor::visit(const SimpleType &simpleType) {
  (void)simpleType;
}

void PropertiesVisitor::visit(const PrimitiveType &primitiveType) {
  (void)primitiveType;
}

void PropertiesVisitor::visit(const Identifier &identifier) {
  name = identifier.getName();
}

std::set<ModifierCode> PropertiesVisitor::getModifiers() {
  return std::move(modifiers);
}

std::string PropertiesVisitor::getName() {
  return std::move(name);
}

} // namespace AST
