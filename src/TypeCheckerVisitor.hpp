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
  void visit(const AST::VariableDeclaration &node) override;
  void visit(const AST::IfThenStatement &node) override;
  void visit(const AST::IfThenElseStatement &node) override;
  void visit(const AST::WhileStatement &node) override;
  void visit(const AST::ForCond &node) override;
  void visit(const AST::ForUpdate &node) override;
  void setReturnType(Env::Type type);
  void setListener(Name::ResolverListener &listener);

private:
  Env::Type visitExpression(const AST::Node &node);
  Checker checker;
  Name::ResolverFactory resolverFactory;
  const Env::TypeLink &typeLink;
  Env::Type returnType;
};

class ExpressionVisitor : public AST::Visitor {
public:
  ExpressionVisitor(const Checker &checker,
                    Name::ResolverFactory &resolverFactory,
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
  Name::ResolverFactory &resolverFactory;
  const Env::TypeLink &typeLink;
  Env::Type type{Env::TypeKeyword::None};
};

} // namespace Type

#endif // TYPECHECKERVISITOR_HPP
