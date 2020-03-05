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

private:
  std::ostream &Stream;
  unsigned int Level;
  void acceptChildrenLevel(const AST::Node &Node);
};

} // namespace AST

#endif // ASTVISITOR_HPP
