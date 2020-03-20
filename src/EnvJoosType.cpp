#include "EnvJoosType.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

std::ostream &operator<<(std::ostream &stream, const JoosType &joosType) {
  stream << "TYPE MODIFIERS: {";
  for (auto const &modifier : joosType.modifiers) {
    stream << modifierName.at(modifier) << " ";
  }
  stream << "}\n\n";
  stream << "Type: " << typeName.at(joosType.type) << ';';
  stream << "Identifier: " << joosType.identifier << "}\n";
  stream << joosType.declare << '\n';
  return stream;
}

void JoosTypeVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void JoosTypeVisitor::visit(const AST::ClassDeclaration &decl) {
  type = Env::Type::Class;
  visitProperties(decl);
}

void JoosTypeVisitor::visit(const AST::InterfaceDeclaration &decl) {
  type = Env::Type::Interface;
  visitProperties(decl);
}

std::set<Env::Modifier> JoosTypeVisitor::getModifiers() {
  return std::move(modifiers);
}

std::string JoosTypeVisitor::getIdentifier() { return identifier; }

Type JoosTypeVisitor::getType() { return type; }

void JoosTypeVisitor::visitProperties(const AST::Node &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  identifier = propertiesVisitor.getIdentifier();
  modifiers = propertiesVisitor.getModifiers();
}

} // namespace Env
