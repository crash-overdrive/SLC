#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"
#include <iostream>

namespace AST {

class Visitor {
public:
  virtual void visit(const Start &Node);
  virtual void visit(const PackageDeclaration &Node);
  virtual void visit(const SingleImportDeclaration &Node);
  virtual void visit(const DemandImportDeclaration &Node);
  virtual void visit(const ClassDeclaration &Node);
  virtual void visit(const InterfaceDeclaration &Node);
  virtual void visit(const FieldDeclaration &Node);
  virtual void visit(const MethodDeclaration &Node);
  virtual void visit(const ConstructorDeclaration &Node);
  virtual void visit(const Modifier &Node);
  virtual void visit(const Identifier &Node);
  virtual void visit(const Extensions &Node);
  virtual void visit(const Name &Node);
  virtual void visit(const PrimitiveType &Node);
  virtual void visit(const ArrayType &Node);
  virtual void visit(const SimpleType &Node);
  virtual void visit(const VoidType &Node);
  virtual void visit(const Expression &Node);
  virtual void visit(const CastExpression &Node);
  virtual void visit(const SingleVariableDeclaration &Node);
  virtual void visit(const Super &Node);
  virtual void visit(const Interfaces &Node);
  virtual void visit(const Block &Node);
  virtual void visit(const IfThenStatement &Node);
  virtual void visit(const IfThenElseStatement &Node);
  virtual void visit(const WhileStatement &Node);
  virtual void visit(const ForStatement &Node);
  virtual void visit(const SimpleStatement &Node);
  virtual void visit(const ClassInstanceCreation &Node);
  virtual void visit(const OperationExpression &Node);
  virtual void visit(const MethodInvocation &Node);
  virtual void visit(const ReturnStatement &Node);
  virtual void visit(const VariableDeclaration &Node);
  virtual void visit(const AssignmentExpression &Node);
  virtual void visit(const ASSIGN &Node);
  virtual void visit(const BinaryOperator &Node);
  virtual void visit(const UnaryOperator &Node);
  virtual void visit(const DecIntLiteral &Node);
  virtual void visit(const BooleanLiteral &Node);
  virtual void visit(const CharacterLiteral &Node);
  virtual void visit(const StringLiteral &Node);
  virtual void visit(const NullLiteral &Node);
  virtual void visit(const ArgumentList &Node);
  virtual void visit(const FieldAccess &Node);
  virtual void visit(const ArrayAccess &Node);
  virtual void visit(const ArrayCreation &Node);
  virtual void visit(const ThisExpression &Node);
  virtual void visit(const ForInit &Node);
  virtual void visit(const ForUpdate &Node);

protected:
  void setError();
  bool error();
  void dispatchChildren(const Node &Parent);

private:
  bool ErrorState = false;
};

class PrintVisitor : public Visitor {
public:
  PrintVisitor(std::ostream &Stream);
  void visit(const Start &Start) override;
  void visit(const PackageDeclaration &Decl) override;
  void visit(const SingleImportDeclaration &Decl) override;
  void visit(const DemandImportDeclaration &Decl) override;
  void visit(const ClassDeclaration &Decl) override;
  void visit(const InterfaceDeclaration &Decl) override;
  void visit(const FieldDeclaration &Decl) override;
  void visit(const ConstructorDeclaration &Decl) override;
  void visit(const Extensions &Extensions) override;
  void visit(const Name &Name) override;
  void visit(const MethodDeclaration &Decl) override;
  void visit(const Modifier &Modifier) override;
  void visit(const Identifier &Identifier) override;
  void visit(const PrimitiveType &PrimitiveType) override;
  void visit(const ArrayType &ArrayType) override;
  void visit(const SimpleType &SimpleType) override;
  void visit(const VoidType &VoidType) override;
  void visit(const Expression &Expression) override;
  void visit(const CastExpression &CastExpression) override;
  void visit(const SingleVariableDeclaration &SingleVariableDeclaration) override;
  void visit(const Super &Super) override;
  void visit(const Interfaces &Interfaces) override;
  void visit(const Block &Block) override;
  void visit(const IfThenStatement &IfThenStatement) override;
  void visit(const IfThenElseStatement &IfThenElseStatement) override;
  void visit(const WhileStatement &WhileStatement) override;
  void visit(const ForStatement &ForStatement) override;
  void visit(const SimpleStatement &SimpleStatement) override;
  void visit(const ClassInstanceCreation &ClassInstanceCreation) override;
  void visit(const OperationExpression &OperationExpression) override;
  void visit(const MethodInvocation &MethodInvocation) override;
  void visit(const ReturnStatement &ReturnStatement) override;
  void visit(const VariableDeclaration &VariableDeclaration) override;
  void visit(const AssignmentExpression &AssignmentExpression) override;
  void visit(const ASSIGN &ASSIGN) override;
  void visit(const BinaryOperator &BinaryOperator) override;
  void visit(const UnaryOperator &UnaryOperator) override;
  void visit(const DecIntLiteral &DecIntLiteral) override;
  void visit(const BooleanLiteral &BooleanLiteral) override;
  void visit(const CharacterLiteral &CharacterLiteral) override;
  void visit(const StringLiteral &StringLiteral) override;
  void visit(const NullLiteral &NullLiteral) override;
  void visit(const ArgumentList &ArgumentList) override;
  void visit(const FieldAccess &FieldAccess) override;
  void visit(const ArrayAccess &ArrayAccess) override;
  void visit(const ArrayCreation &ArrayCreation) override;
  void visit(const ThisExpression &ThisExpression) override;
  void visit(const ForInit &ForInit) override;
  void visit(const ForUpdate &ForUpdate) override;

private:
  std::ostream &Stream;
  unsigned int Level;
  void acceptChildrenLevel(const AST::Node &Node);
};

} // namespace AST

#endif // ASTVISITOR_HPP
