#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "ASTNode.hpp"
#include <iostream>

namespace AST {

class Visitor {
public:
  virtual void visit(const Start &Start);
  virtual void visit(const ClassDeclaration &Decl);
  virtual void visit(const InterfaceDeclaration &Decl);
  virtual void visit(const FieldDeclaration &Decl);
  virtual void visit(const MethodDeclaration &Decl);
  virtual void visit(const Modifier &Modifier);
  virtual void visit(const Identifier &Identifier);
};

class PrintVisitor : public Visitor {
public:
  PrintVisitor(std::ostream &Stream);
  void visit(const Start &Start) override;
  void visit(const ClassDeclaration &Decl) override;
  void visit(const InterfaceDeclaration &Decl) override;
  void visit(const FieldDeclaration &Decl) override;
  void visit(const MethodDeclaration &Decl) override;
  void visit(const Modifier &Modifier) override;
  void visit(const Identifier &Identifier) override;

private:
  std::ostream &Stream;
  unsigned int Level;
  void acceptChildren(const AST::Node &Node);
};

} // namespace AST

#endif // ASTVISITOR_HPP
