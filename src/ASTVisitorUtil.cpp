#include "ASTVisitorUtil.hpp"

namespace AST {

void NameVisitor::visit(const AST::Name &name) { dispatchChildren(name); }

void NameVisitor::visit(const Identifier &identifier) {
  name.emplace_back(identifier.getName());
}

void NameVisitor::visit(const PrimitiveType &primitiveType) {
  name.emplace_back(primitiveType.getType());
}

std::vector<std::string> NameVisitor::getName() { return std::move(name); }

void PropertiesVisitor::visit(const Modifier &modifier) {
  modifiers.emplace(modifier.getCode());
}

void PropertiesVisitor::visit(const PrimitiveType &primitiveType) {
  variableDescriptor = Env::VariableDescriptor{Env::VariableType::SimpleType,
                                               {primitiveType.getType()}};
}

void PropertiesVisitor::visit(const VoidType &voidType) {
  variableDescriptor = Env::VariableDescriptor{Env::VariableType::SimpleType,
                                               {voidType.getType()}};
}

void PropertiesVisitor::visit(const SimpleType &simpleType) {
  NameVisitor nameVisitor;
  nameVisitor.dispatchChildren(simpleType);
  variableDescriptor = Env::VariableDescriptor{Env::VariableType::SimpleType,
                                               nameVisitor.getName()};
}

void PropertiesVisitor::visit(const ArrayType &arrayType) {
  NameVisitor nameVisitor;
  nameVisitor.dispatchChildren(arrayType);
  variableDescriptor = Env::VariableDescriptor{Env::VariableType::ArrayType,
                                               nameVisitor.getName()};
}

void PropertiesVisitor::visit(const Identifier &node) {
  identifier = node.getName();
}

std::set<ModifierCode> PropertiesVisitor::getModifiers() {
  return std::move(modifiers);
}

std::string PropertiesVisitor::getIdentifier() { return std::move(identifier); }

Env::VariableDescriptor PropertiesVisitor::getVariableDescriptor() {
  return variableDescriptor;
}

void ArgumentsVisitor::visit(const SingleVariableDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);
  args.push_back(propertiesVisitor.getVariableDescriptor());
}

std::vector<Env::VariableDescriptor> ArgumentsVisitor::getArgs() {
  return std::move(args);
}

} // namespace AST
