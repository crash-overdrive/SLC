#include "TypeCheckerVisitor.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeVisitorUtil.hpp"

namespace Type {

StatementVisitor::StatementVisitor(const Env::TypeLink &typeLink,
                                   const Env::PackageTree &tree)
    : LocalTrackVisitor(typeLink), checker(tree),
      resolverFactory(getLocal(), typeLink), typeLink(typeLink) {}

StatementVisitor::StatementVisitor(const Env::TypeLink &typeLink,
                                   const Env::PackageTree &tree,
                                   Name::ResolverListener &listener)
    : LocalTrackVisitor(typeLink), checker(tree),
      resolverFactory(getLocal(), typeLink, listener), typeLink(typeLink) {}

void StatementVisitor::visit(const AST::ReturnStatement &node) {
  Env::Type expressionReturnType{visitExpression(node)};
  if (returnType == Env::TypeKeyword::Void &&
      expressionReturnType == Env::TypeKeyword::None) {
    return;
  }
  if (!checker.checkAssignment(returnType, expressionReturnType)) {
    setError();
  }
}

void StatementVisitor::visit(const AST::ExpressionStatement &node) {
  visitExpression(node);
}

void StatementVisitor::visit(const AST::VariableDeclaration &node) {
  dispatchChildren(node);
  auto variable = getLocal().getLastVariable();
  Env::Type expressionType{visitExpression(node)};
  if (!checker.checkAssignment(variable.second, expressionType)) {
    setError();
  }
}

void StatementVisitor::visit(const AST::IfThenStatement &node) {
  if (visitExpression(node) != Env::TypeKeyword::Boolean) {
    setError();
    return;
  }
  dispatchChildren(node);
}

void StatementVisitor::visit(const AST::IfThenElseStatement &node) {
  if (visitExpression(node) != Env::TypeKeyword::Boolean) {
    setError();
    return;
  }
  dispatchChildren(node);
}

void StatementVisitor::visit(const AST::WhileStatement &node) {
  if (visitExpression(node) != Env::TypeKeyword::Boolean) {
    setError();
    return;
  }
  dispatchChildren(node);
}

void StatementVisitor::visit(const AST::ForCond &node) {
  if (visitExpression(node) != Env::TypeKeyword::Boolean) {
    setError();
    return;
  }
}

void StatementVisitor::visit(const AST::ForUpdate &node) {
  visitExpression(node);
}

Env::Type StatementVisitor::visitExpression(const AST::Node &node) {
  ExpressionVisitor visitor(checker, resolverFactory, typeLink);
  visitor.dispatchChildren(node);
  if (visitor.isErrorState()) {
    setError();
  }
  return visitor.getType();
}

void StatementVisitor::setReturnType(Env::Type type) { returnType = type; }

void StatementVisitor::setListener(Name::ResolverListener &listener) {
  resolverFactory.setListener(listener);
}

ExpressionVisitor::ExpressionVisitor(const Checker &checker,
                                     Name::ResolverFactory &resolverFactory,
                                     const Env::TypeLink &typeLink)
    : checker(checker), resolverFactory(resolverFactory), typeLink(typeLink) {}

void ExpressionVisitor::visit(const AST::AssignmentExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolverFactory, typeLink);
  binaryVisitor.dispatchChildren(node);
  setType(
      checker.checkAssignment(binaryVisitor.getLHS(), binaryVisitor.getRHS()));
}

void ExpressionVisitor::visit(const AST::BinaryExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolverFactory, typeLink);
  binaryVisitor.dispatchChildren(node);
  BinaryOperatorVisitor operatorVisitor;
  operatorVisitor.dispatchChildren(node);
  BinaryOperation operation{operatorVisitor.getBinaryOperator(),
                            binaryVisitor.getLHS(), binaryVisitor.getRHS()};
  setType(checker.checkBinaryOperation(operation));
}

void ExpressionVisitor::visit(const AST::UnaryExpression &node) {
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);
  UnaryOperatorVisitor operatorVisitor;
  operatorVisitor.dispatchChildren(node);
  UnaryOperation operation{operatorVisitor.getUnaryOperator(),
                           expressionVisitor.getType()};
  setType(checker.checkUnaryOperation(operation));
}

void ExpressionVisitor::visit(const AST::CastType &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);
  setType(
      checker.checkCasting(typeVisitor.getType(), expressionVisitor.getType()));
}

void ExpressionVisitor::visit(const AST::CastExpression &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolverFactory, typeLink);
  binaryVisitor.setCastExpression();
  binaryVisitor.dispatchChildren(node);
  setType(checker.checkCasting(binaryVisitor.getLHS(), binaryVisitor.getRHS()));
}

void ExpressionVisitor::visit(const AST::InstanceOfExpression &node) {
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  setType(checker.checkInstanceOf(expressionVisitor.getType(),
                                  typeVisitor.getType()));
}

void ExpressionVisitor::visit(const AST::ArrayCreation &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);
  setType(checker.checkArrayCreation(typeVisitor.getType(),
                                     expressionVisitor.getType()));
}

void ExpressionVisitor::visit(const AST::ArrayAccess &node) {
  BinaryExpressionVisitor binaryVisitor(checker, resolverFactory, typeLink);
  binaryVisitor.dispatchChildren(node);
  setType(
      checker.checkArrayAccess(binaryVisitor.getLHS(), binaryVisitor.getRHS()));
}

void ExpressionVisitor::visit(const AST::ClassInstanceCreation &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(node);
  ArgumentsVisitor argumentsVisitor(checker, resolverFactory, typeLink);
  argumentsVisitor.dispatchChildren(node);

  Name::ConstructorResolver resolver = resolverFactory.getConstructor();
  setType(resolver.match(typeVisitor.getType(), argumentsVisitor.getArgs()));
}

void ExpressionVisitor::visit(const AST::FieldAccess &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);

  Name::FieldResolver resolver = resolverFactory.getField();
  setType(resolver.match(expressionVisitor.getType(),
                         propertiesVisitor.getIdentifier()));
}

void ExpressionVisitor::visit(const AST::MethodNameInvocation &node) {
  AST::NameVisitor nameVisitor;
  nameVisitor.dispatchChildren(node);
  ArgumentsVisitor argumentsVisitor(checker, resolverFactory, typeLink);
  argumentsVisitor.dispatchChildren(node);

  Name::MethodResolver resolver = resolverFactory.getMethod();
  resolver.setArgs(argumentsVisitor.getArgs());
  setType(resolver.match(nameVisitor.getName()));
}

void ExpressionVisitor::visit(const AST::MethodPrimaryInvocation &node) {
  ExpressionVisitor expressionVisitor(checker, resolverFactory, typeLink);
  expressionVisitor.dispatchChildren(node);
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  ArgumentsVisitor argumentsVisitor(checker, resolverFactory, typeLink);
  argumentsVisitor.dispatchChildren(node);

  Name::MethodResolver resolver = resolverFactory.getMethod();
  resolver.setArgs(argumentsVisitor.getArgs());
  setType(resolver.match(expressionVisitor.getType(),
                         propertiesVisitor.getIdentifier()));
}

void ExpressionVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);

  Name::FieldResolver resolver = resolverFactory.getField();
  setType(resolver.match(visitor.getName()));
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
