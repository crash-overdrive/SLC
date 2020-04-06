#include "TypeCheckerVisitor.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeVisitorUtil.hpp"

namespace Type {

StatementVisitor::StatementVisitor(const Env::TypeLink &typeLink,
                                   const Env::PackageTree &tree)
    : LocalTrackVisitor(typeLink), checker(tree),
      resolver(getLocal(), typeLink), typeLink(typeLink) {}

void StatementVisitor::visit(const AST::ReturnStatement &node) {
  ExpressionVisitor visitor(checker, resolver, typeLink);
  visitor.dispatchChildren(node);
  if (visitor.isErrorState()) {
    setError();
    return;
  }
  returnType = visitor.getType();
}

ExpressionVisitor::ExpressionVisitor(const Checker &checker,
                                     const Name::Resolver &resolver,
                                     const Env::TypeLink &typeLink)
    : checker(checker), resolver(resolver), typeLink(typeLink) {}

void ExpressionVisitor::visit(const AST::AssignmentExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolver, typeLink);
  binaryVisitor.dispatchChildren(node);
  setType(
      checker.checkAssignment(binaryVisitor.getLHS(), binaryVisitor.getRHS()));
}

void ExpressionVisitor::visit(const AST::BinaryExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolver, typeLink);
  binaryVisitor.dispatchChildren(node);
  BinaryOperatorVisitor operatorVisitor;
  operatorVisitor.dispatchChildren(node);
  BinaryOperation operation{operatorVisitor.getBinaryOperator(),
                            binaryVisitor.getLHS(), binaryVisitor.getRHS()};
  setType(checker.checkBinaryOperation(operation));
}

void ExpressionVisitor::visit(const AST::CastType &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolver, typeLink);
  expressionVisitor.dispatchChildren(node);
  setType(
      checker.checkCasting(typeVisitor.getType(), expressionVisitor.getType()));
}

void ExpressionVisitor::visit(const AST::CastExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolver, typeLink);
  binaryVisitor.setCastExpression();
  binaryVisitor.dispatchChildren(node);
  setType(checker.checkCasting(binaryVisitor.getLHS(), binaryVisitor.getRHS()));
}

void ExpressionVisitor::visit(const AST::ArrayCreation &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolver, typeLink);
  expressionVisitor.dispatchChildren(node);
  setType(checker.checkArrayCreation(typeVisitor.getType(),
                                     expressionVisitor.getType()));
}

void ExpressionVisitor::visit(const AST::FieldAccess &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolver, typeLink);
  expressionVisitor.dispatchChildren(node);
  setType(resolver.findField(expressionVisitor.getType(),
                             propertiesVisitor.getIdentifier()));
}

void ExpressionVisitor::visit(const AST::MethodNameInvocation &node) {
  AST::NameVisitor nameVisitor;
  nameVisitor.dispatchChildren(node);
  ArgumentsVisitor argumentsVisitor(checker, resolver, typeLink);
  argumentsVisitor.dispatchChildren(node);
  setType(
      resolver.findMethod(nameVisitor.getName(), argumentsVisitor.getArgs()));
}

void ExpressionVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  setType(resolver.findField(visitor.getName()));
}

void ExpressionVisitor::visit(const AST::DecIntLiteral &) {
  type = Env::TypeKeyword::Integer;
}

void ExpressionVisitor::visit(const AST::BooleanLiteral &) {
  type = Env::TypeKeyword::Boolean;
}

void ExpressionVisitor::visit(const AST::CharacterLiteral &) {
  type = Env::TypeKeyword::Character;
}

void ExpressionVisitor::visit(const AST::NullLiteral &) {
  type = Env::TypeKeyword::Null;
}

void ExpressionVisitor::visit(const AST::ThisExpression &) {
  type = Env::Type(&typeLink.getDeclaration());
}

void ExpressionVisitor::visit(const AST::StringLiteral &) {
  type = Env::Type(typeLink.find({"java", "lang", "String"}));
}

Env::Type ExpressionVisitor::getType() { return std::move(type); }

void ExpressionVisitor::setType(std::optional<Env::Type> result) {
  if (!result) {
    setError();
  } else {
    type = *result;
  }
}

} // namespace Type
