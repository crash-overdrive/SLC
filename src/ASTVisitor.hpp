#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"
#include <iostream>

namespace AST {

class Visitor {
public:
  virtual void visit(const Start &);
  virtual void visit(const PackageDeclaration &);
  virtual void visit(const SingleImportDeclaration &);
  virtual void visit(const DemandImportDeclaration &);
  virtual void visit(const ClassDeclaration &);
  virtual void visit(const InterfaceDeclaration &);
  virtual void visit(const FieldDeclaration &);
  virtual void visit(const MethodDeclaration &);
  virtual void visit(const ConstructorDeclaration &);
  virtual void visit(const Modifier &);
  virtual void visit(const Identifier &);
  virtual void visit(const Extensions &);
  virtual void visit(const Name &);
  virtual void visit(const PrimitiveType &);
  virtual void visit(const ArrayType &);
  virtual void visit(const SimpleType &);
  virtual void visit(const VoidType &);
  virtual void visit(const FormalParameterList &);
  virtual void visit(const SingleVariableDeclaration &);
  virtual void visit(const Super &);
  virtual void visit(const Interfaces &);
  virtual void visit(const Block &);
  virtual void visit(const IfThenStatement &);
  virtual void visit(const IfThenElseStatement &);
  virtual void visit(const WhileStatement &);
  virtual void visit(const ForStatement &);
  virtual void visit(const SimpleStatement &);
  virtual void visit(const ClassInstanceCreation &);
  virtual void visit(const OperationExpression &);
  virtual void visit(const MethodInvocation &);
  virtual void visit(const ReturnStatement &);
  virtual void visit(const VariableDeclarationStatement &);
  virtual void visit(const AssignmentExpression &);
  virtual void visit(const ASSIGN &);
  virtual void visit(const BinaryOperator &);
  virtual void visit(const UnaryOperator &);
  virtual void visit(const DecIntLiteral &);
  virtual void visit(const BooleanLiteral &);
  virtual void visit(const CharacterLiteral &);
  virtual void visit(const StringLiteral &);
  virtual void visit(const NullLiteral &);
  virtual void visit(const ArgumentList &);
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
  void visit(const FormalParameterList &FormalParameterList) override;
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
  void visit(const VariableDeclarationStatement &VariableDeclarationStatement) override;
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

private:
  std::ostream &Stream;
  unsigned int Level;
  void acceptChildrenLevel(const AST::Node &Node);
};

} // namespace AST

#endif // ASTVISITOR_HPP
