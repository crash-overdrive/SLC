#ifndef TYPEVISITORUTIL_HPP
#define TYPEVISITORUTIL_HPP

#include "ASTVisitor.hpp"
#include "NameResolver.hpp"
#include "TypeChecker.hpp"

namespace Type {

class BinaryExpressionVisitor : public AST::Visitor {
public:
  BinaryExpressionVisitor(const Checker &checker,
                          Name::ResolverFactory &resolverFactory,
                          const Env::TypeLink &typeLink);
  // Skip operator visit
  void visit(const AST::Operator &node) override;
  void visit(const AST::Name &node) override;
  void setCastExpression();
  Env::Type getLHS();
  Env::Type getRHS();

private:
  void postVisit(const AST::Node &node) override;
  const Checker &checker;
  Name::ResolverFactory &resolverFactory;
  const Env::TypeLink &typeLink;
  bool castExpression = false;
  Env::Type lhs;
  Env::Type rhs;
};

class BinaryOperatorVisitor : public AST::Visitor {
public:
  void visit(const AST::Operator &node) override;
  BinaryOperator getBinaryOperator();

private:
  BinaryOperator binaryOperator;
};

class UnaryOperatorVisitor : public AST::Visitor {
public:
  void visit(const AST::Operator &node) override;
  UnaryOperator getUnaryOperator();

private:
  UnaryOperator unaryOperator;
};

class ArgumentsVisitor : public AST::Visitor {
public:
  ArgumentsVisitor(const Checker &checker,
                   Name::ResolverFactory &resolverFactory,
                   const Env::TypeLink &typeLink);
  void visit(const AST::Argument &node) override;
  std::vector<Env::Type> getArgs();

private:
  const Checker &checker;
  Name::ResolverFactory &resolverFactory;
  const Env::TypeLink &typeLink;
  std::vector<Env::Type> args;
};

class FieldVisitor : public AST::TrackVisitor {
public:
  FieldVisitor(std::string identifier,
               const std::unordered_set<std::string> &declare,
               const Env::TypeBody &body);
  void visit(const AST::SimpleType &node) override;
  void visit(const AST::AssignmentExpression &node) override;
  void visit(const AST::Name &name) override;

private:
  void postVisit(const AST::Node &parent) override;
  bool lhsAssignment = false;
  std::string identifier;
  const std::unordered_set<std::string> &declare;
  const Env::TypeBody &body;
};

class StaticThisVisitor : public AST::TrackVisitor {
public:
  void visit(const AST::ThisExpression &node) override;
  bool isErrorState() const;

private:
  bool errorState = false;
};

class StaticListener : public Name::ResolverListener {
public:
  void listenImplicit() override;
  bool isErrorState() const;

private:
  bool errorState = false;
};

} // namespace Type

#endif // TYPEVISITORUTIL_HPP
