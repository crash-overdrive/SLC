#include "EnvTypeLink.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

void TypeLinkVisitor::visit(const AST::SingleImportDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  SingleImports.emplace_back(Visitor.getName());
};

void TypeLinkVisitor::visit(const AST::DemandImportDeclaration &Decl) {
  AST::NameVisitor Visitor;
  Visitor.dispatchChildren(Decl);
  DemandImports.emplace_back(Visitor.getName());
};

void TypeLinkVisitor::visit(const AST::InterfaceDeclaration &){};

void TypeLinkVisitor::visit(const AST::ClassDeclaration &){};

std::vector<std::vector<std::string>>
TypeLinkVisitor::getSingleImports() const {
  return std::move(SingleImports);
}

std::vector<std::vector<std::string>>
TypeLinkVisitor::getDemandImports() const {
  return std::move(DemandImports);
}

} // namespace Env
