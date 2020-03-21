#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"
#include <fstream>
#include <iostream>

namespace AST {

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(const Start &node);
  virtual void visit(const PackageDeclaration &node);
  virtual void visit(const SingleImportDeclaration &node);
  virtual void visit(const DemandImportDeclaration &node);
  virtual void visit(const ClassDeclaration &node);
  virtual void visit(const InterfaceDeclaration &node);
  virtual void visit(const FieldDeclaration &node);
  virtual void visit(const MethodDeclaration &node);
  virtual void visit(const ConstructorDeclaration &node);
  virtual void visit(const Modifier &node);
  virtual void visit(const Identifier &node);
  virtual void visit(const Extensions &node);
  virtual void visit(const Name &node);
  virtual void visit(const PrimitiveType &node);
  virtual void visit(const ArrayType &node);
  virtual void visit(const SimpleType &node);
  virtual void visit(const VoidType &node);
  virtual void visit(const Expression &node);
  virtual void visit(const CastExpression &node);
  virtual void visit(const SingleVariableDeclaration &node);
  virtual void visit(const Super &node);
  virtual void visit(const Interfaces &node);
  virtual void visit(const Block &node);
  virtual void visit(const IfThenStatement &node);
  virtual void visit(const IfThenElseStatement &node);
  virtual void visit(const WhileStatement &node);
  virtual void visit(const ForStatement &node);
  virtual void visit(const SimpleStatement &node);
  virtual void visit(const ClassInstanceCreation &node);
  virtual void visit(const OperationExpression &node);
  virtual void visit(const MethodInvocation &node);
  virtual void visit(const ReturnStatement &node);
  virtual void visit(const VariableDeclaration &node);
  virtual void visit(const AssignmentExpression &node);
  virtual void visit(const ASSIGN &node);
  virtual void visit(const BinaryOperator &node);
  virtual void visit(const UnaryOperator &node);
  virtual void visit(const DecIntLiteral &node);
  virtual void visit(const BooleanLiteral &node);
  virtual void visit(const CharacterLiteral &node);
  virtual void visit(const StringLiteral &node);
  virtual void visit(const NullLiteral &node);
  virtual void visit(const ArgumentList &node);
  virtual void visit(const FieldAccess &node);
  virtual void visit(const ArrayAccess &node);
  virtual void visit(const ArrayCreation &node);
  virtual void visit(const ThisExpression &node);
  virtual void visit(const ForInit &node);
  virtual void visit(const ForUpdate &node);
  virtual void dispatchChildren(const Node &parent);

protected:
  virtual void postVisit(const Node &parent);
};

class PrintVisitor : public Visitor {
public:
  PrintVisitor(std::ostream &stream);
  void visit(const Start &) override;
  void visit(const PackageDeclaration &) override;
  void visit(const SingleImportDeclaration &) override;
  void visit(const DemandImportDeclaration &) override;
  void visit(const ClassDeclaration &) override;
  void visit(const InterfaceDeclaration &) override;
  void visit(const FieldDeclaration &) override;
  void visit(const ConstructorDeclaration &) override;
  void visit(const Extensions &) override;
  void visit(const Name &) override;
  void visit(const MethodDeclaration &) override;
  void visit(const Modifier &) override;
  void visit(const Identifier &identifier) override;
  void visit(const PrimitiveType &primitiveType) override;
  void visit(const SimpleType &simpleType) override;
  void visit(const ArrayType &) override;
  void visit(const VoidType &voidType) override;
  void visit(const Expression &) override;
  void visit(const CastExpression &) override;
  void visit(const SingleVariableDeclaration &) override;
  void visit(const Super &) override;
  void visit(const Interfaces &) override;
  void visit(const Block &) override;
  void visit(const IfThenStatement &) override;
  void visit(const IfThenElseStatement &) override;
  void visit(const WhileStatement &) override;
  void visit(const ForStatement &) override;
  void visit(const SimpleStatement &) override;
  void visit(const ClassInstanceCreation &) override;
  void visit(const OperationExpression &) override;
  void visit(const MethodInvocation &) override;
  void visit(const ReturnStatement &) override;
  void visit(const VariableDeclaration &) override;
  void visit(const AssignmentExpression &) override;
  void visit(const ASSIGN &assign) override;
  void visit(const BinaryOperator &binaryOperator) override;
  void visit(const UnaryOperator &unaryOperator) override;
  void visit(const DecIntLiteral &decIntLiteral) override;
  void visit(const BooleanLiteral &booleanLiteral) override;
  void visit(const CharacterLiteral &characterLiteral) override;
  void visit(const StringLiteral &stringLiteral) override;
  void visit(const NullLiteral &nullLiteral) override;
  void visit(const ArgumentList &) override;
  void visit(const FieldAccess &) override;
  void visit(const ArrayAccess &) override;
  void visit(const ArrayCreation &) override;
  void visit(const ThisExpression &thisExpression) override;
  void visit(const ForInit &) override;
  void visit(const ForUpdate &) override;

private:
  std::ostream &stream;
};

class TrackVisitor : public Visitor {
public:
  void setLog(std::ostream &stream);
  bool isErrorState();

protected:
  void setError();
  void postVisit(const Node &parent) override;

private:
  bool errorState = false;
  unsigned int level = 0;
  std::ofstream nullStream{};
  std::reference_wrapper<std::ostream> streamRef = nullStream;
};

} // namespace AST

#endif // ASTVISITOR_HPP
