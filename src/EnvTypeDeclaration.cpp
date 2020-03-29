#include "EnvTypeDeclaration.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

TypeDeclaration::TypeDeclaration(std::set<Modifier> modifiers,
                                 DeclarationKeyword keyword,
                                 std::string identifier,
                                 std::unique_ptr<AST::Node> astNode)
    : modifiers(std::move(modifiers)), keyword(keyword),
      identifier(std::move(identifier)), astNode(std::move(astNode)) {}

std::ostream &operator<<(std::ostream &stream, const TypeDeclaration &decl) {
  stream << "TYPE MODIFIERS: {";
  for (auto const &modifier : decl.modifiers) {
    stream << modifierString.at(modifier) << " ";
  }
  stream << "}\n\n";
  stream << "Type: " << declareKeywordString.at(decl.keyword) << "\n";
  stream << "Identifier: " << decl.identifier << "\n";
  stream << decl.body << '\n';
  return stream;
}

void TypeDeclarationVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void TypeDeclarationVisitor::visit(const AST::ClassDeclaration &decl) {
  keyword = DeclarationKeyword::Class;
  visitProperties(decl);
}

void TypeDeclarationVisitor::visit(const AST::InterfaceDeclaration &decl) {
  keyword = DeclarationKeyword::Interface;
  visitProperties(decl);
}

std::set<Env::Modifier> TypeDeclarationVisitor::getModifiers() {
  return std::move(modifiers);
}

std::string TypeDeclarationVisitor::getIdentifier() { return identifier; }

DeclarationKeyword TypeDeclarationVisitor::getKeyword() { return keyword; }

void TypeDeclarationVisitor::visitProperties(const AST::Node &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  identifier = propertiesVisitor.getIdentifier();
  modifiers = propertiesVisitor.getModifiers();
}

} // namespace Env
