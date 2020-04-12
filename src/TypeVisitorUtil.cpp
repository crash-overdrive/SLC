#include "TypeVisitorUtil.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeCheckerVisitor.hpp"

namespace Type {

BinaryExpressionVisitor::BinaryExpressionVisitor(
    const Checker &checker, Name::ResolverFactory &resolverFactory,
    const Env::TypeLink &typeLink)
    : checker(checker), resolverFactory(resolverFactory), typeLink(typeLink) {}

void BinaryExpressionVisitor::visit(const AST::Operator &) {}

void BinaryExpressionVisitor::visit(const AST::Name &node) {
  if (castExpression && lhs.keyword == Env::TypeKeyword::Void) {
    AST::NameVisitor nameVisitor;
    node.accept(nameVisitor);
    lhs = Env::Type(typeLink.find(nameVisitor.getName()));
  } else {
    postVisit(node);
  }
}

void BinaryExpressionVisitor::setCastExpression() { castExpression = true; }

void BinaryExpressionVisitor::postVisit(const AST::Node &node) {
  ExpressionVisitor visitor(checker, resolverFactory, typeLink);
  node.accept(visitor);
  if (lhs.keyword == Env::TypeKeyword::Void) {
    lhs = visitor.getType();
  } else {
    rhs = visitor.getType();
  }
}

Env::Type BinaryExpressionVisitor::getLHS() { return std::move(lhs); }

Env::Type BinaryExpressionVisitor::getRHS() { return std::move(rhs); }

void BinaryOperatorVisitor::visit(const AST::Operator &node) {
  binaryOperator = symbolBinaryOperator.at(node.getOperatorSymbol());
}

BinaryOperator BinaryOperatorVisitor::getBinaryOperator() {
  return binaryOperator;
}

void UnaryOperatorVisitor::visit(const AST::Operator &node) {
  unaryOperator = symbolUnaryOperator.at(node.getOperatorSymbol());
}

UnaryOperator UnaryOperatorVisitor::getUnaryOperator() {
  return std::move(unaryOperator);
}

ArgumentsVisitor::ArgumentsVisitor(const Checker &checker,
                                   Name::ResolverFactory &resolverFactory,
                                   const Env::TypeLink &typeLink)
    : checker(checker), resolverFactory(resolverFactory), typeLink(typeLink) {}

void ArgumentsVisitor::visit(const AST::Argument &node) {
  ExpressionVisitor visitor(checker, resolverFactory, typeLink);
  visitor.dispatchChildren(node);
  args.emplace_back(visitor.getType());
}

std::vector<Env::Type> ArgumentsVisitor::getArgs() { return std::move(args); }

SelfReferenceVisitor::SelfReferenceVisitor(std::string identifier)
    : identifier(std::move(identifier)) {}

void SelfReferenceVisitor::visit(const AST::SimpleType &) {}

void SelfReferenceVisitor::visit(const AST::AssignmentExpression &node) {
  lhsAssignment = true;
  dispatchChildren(node);
}

void SelfReferenceVisitor::visit(const AST::Name &name) {
  if (lhsAssignment) {
    lhsAssignment = false;
    return;
  }
  AST::NameVisitor visitor;
  name.accept(visitor);
  std::vector<std::string> fullName = visitor.getName();
  if (fullName.size() == 1 && fullName.at(0) == identifier) {
    setError();
  }
}

void SelfReferenceVisitor::postVisit(const AST::Node &parent) {
  lhsAssignment = false;
  dispatchChildren(parent);
}

void FieldInitializerListener::listenField(const Env::Field &field) {
  if (field.declaration == &decl) {
    errorState = true;
  }
}

bool FieldInitializerListener::isErrorState() const { return errorState; }

} // namespace Type
