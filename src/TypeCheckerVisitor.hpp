#ifndef TYPECHECKERVISITOR_HPP
#define TYPECHECKERVISITOR_HPP

#include "TypeVisitorUtil.hpp"

namespace Type {

class StatementVisitor : public Env::LocalTrackVisitor {
public:
  using LocalTrackVisitor::visit;
  StatementVisitor(const Env::TypeLink &typeLink, const Env::PackageTree &tree);
  void visit(const AST::ReturnStatement &node) override;
  void visit(const AST::ExpressionStatement &node) override;

private:
  Env::Type visitExpression(const AST::Node &node);
  Checker checker;
  Name::Resolver resolver;
  const Env::TypeLink &typeLink;
};

class ExpressionVisitor : public AST::Visitor {
public:
  ExpressionVisitor(const Checker &checker, const Name::Resolver &resolver,
                    const Env::TypeLink &typeLink);
  void visit(const AST::AssignmentExpression &node) override;
  void visit(const AST::BinaryExpression &node) override;
  void visit(const AST::UnaryExpression &node) override;
  void visit(const AST::CastType &node) override;
  void visit(const AST::CastExpression &node) override;
  void visit(const AST::InstanceOfExpression &node) override;
  void visit(const AST::ArrayCreation &node) override;
  void visit(const AST::ArrayAccess &node) override;
  void visit(const AST::ClassInstanceCreation &node) override;
  void visit(const AST::FieldAccess &node) override;
  void visit(const AST::MethodNameInvocation &node) override;
  void visit(const AST::MethodPrimaryInvocation &node) override;
  void visit(const AST::Name &node) override;
  void visit(const AST::DecIntLiteral &node) override;
  void visit(const AST::BooleanLiteral &node) override;
  void visit(const AST::CharacterLiteral &node) override;
  void visit(const AST::NullLiteral &node) override;
  void visit(const AST::ThisExpression &node) override;
  void visit(const AST::StringLiteral &node) override;
  Env::Type getType();

private:
  void setType(std::optional<Env::Type> result);
  const Checker &checker;
  const Name::Resolver &resolver;
  const Env::TypeLink &typeLink;
  Env::Type type;
};

} // namespace Type

#endif // TYPECHECKERVISITOR_HPP
