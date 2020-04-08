#include "TypeVisitorUtil.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeCheckerVisitor.hpp"

namespace Type {

BinaryExpressionVisitor::BinaryExpressionVisitor(const Checker &checker,
                                                 const Name::Resolver &resolver,
                                                 const Env::TypeLink &typeLink)
    : checker(checker), resolver(resolver), typeLink(typeLink) {}

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
  ExpressionVisitor visitor(checker, resolver, typeLink);
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
                                   const Name::Resolver &resolver,
                                   const Env::TypeLink &typeLink)
    : checker(checker), resolver(resolver), typeLink(typeLink) {}

void ArgumentsVisitor::visit(const AST::Argument &node) {
  ExpressionVisitor visitor(checker, resolver, typeLink);
  visitor.dispatchChildren(node);
  args.emplace_back(visitor.getType());
}

std::vector<Env::Type> ArgumentsVisitor::getArgs() { return std::move(args); }

SelfInitializeVisitor::SelfInitializeVisitor(const std::string &identifier)
    : identifier(identifier) {}

void SelfInitializeVisitor::visit(const AST::SimpleType &) {}

void SelfInitializeVisitor::visit(const AST::AssignmentExpression &node) {
  lhsAssignment = true;
  dispatchChildren(node);
}

void SelfInitializeVisitor::visit(const AST::Name &name) {
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

void SelfInitializeVisitor::postVisit(const AST::Node &parent) {
  lhsAssignment = false;
  dispatchChildren(parent);
}

} // namespace Type
