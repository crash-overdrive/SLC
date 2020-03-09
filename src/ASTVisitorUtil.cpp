#include "ASTVisitorUtil.hpp"

namespace AST {

void NameVisitor::visit(const AST::Name &name) {
  dispatchChildren(name);
}

void NameVisitor::visit(const Identifier &identifier) {
  name.emplace_back(identifier.getName());
}

std::vector<std::string> NameVisitor::getName() {
  return name;
}

std::set<ModifierCode> PropertiesVisitor::getModifiers() {
  return modifiers;
}

std::string PropertiesVisitor::getName() {
  return name;
}

} // namespace AST
