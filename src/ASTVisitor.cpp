#include "ASTVisitor.hpp"

void AST::Visitor::visit(const Start &Start) { (void)Start; }
void AST::Visitor::visit(const TypeDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const FieldDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const MethodDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const Modifier &Modifier) { (void)Modifier; }
void AST::Visitor::visit(const Identifier &Identifier) { (void)Identifier; }

AST::PrintVisitor::PrintVisitor(std::ostream &Stream)
    : Stream(Stream), Level(0) {}

void AST::PrintVisitor::visit(const Start &Start) {
  Stream << "Start:" << '\n';
  acceptChildren(Start);
}

void AST::PrintVisitor::visit(const TypeDeclaration &Decl) {
  Stream << "TypeDeclaration: Interface: " << std::boolalpha
         << Decl.isInterface() << '\n';
  acceptChildren(Decl);
}

void AST::PrintVisitor::visit(const FieldDeclaration &Decl) {
  Stream << "FieldDeclaration:" << '\n';
  acceptChildren(Decl);
}

void AST::PrintVisitor::visit(const MethodDeclaration &Decl) {
  Stream << "MethodDeclaration:" << '\n';
  acceptChildren(Decl);
}

void AST::PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode()) << '\n';
  acceptChildren(Modifier);
}

void AST::PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName() << '\n';
}

void AST::PrintVisitor::acceptChildren(const AST::Node &Node) {
  for (const auto &Child : Node.getChildren()) {
    ++Level;
    Stream << std::string(Level * 2, ' ');
    Child->accept(*this);
    --Level;
  }
}
