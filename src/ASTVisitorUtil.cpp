#include "ASTVisitorUtil.hpp"

namespace AST {

void NameVisitor::visit(const AST::Name &Name) {
  dispatchChildren(Name);
}

void NameVisitor::visit(const Identifier &Identifier) {
  Name.emplace_back(Identifier.getName());
}

std::vector<std::string> NameVisitor::getName() {
  return Name;
}

} // namespace AST
