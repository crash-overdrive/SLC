#include "ASTVisitor.hpp"

void AST::Visitor::visit(const Start &Start) { (void)Start; }
void AST::Visitor::visit(const InterfaceDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const PackageDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const SingleImportDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const DemandImportDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const ClassDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const FieldDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const MethodDeclaration &Decl) { (void)Decl; }
void AST::Visitor::visit(const Modifier &Modifier) { (void)Modifier; }
void AST::Visitor::visit(const Identifier &Identifier) { (void)Identifier; }

AST::PrintVisitor::PrintVisitor(std::ostream &Stream)
    : Stream(Stream), Level(0) {}

void AST::PrintVisitor::visit(const Start &Start) {
  Stream << "Start" << '\n';
  acceptChildrenLevel(Start);
}

void AST::PrintVisitor::visit(const PackageDeclaration &Decl) {
  Stream << "PackageDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const SingleImportDeclaration &Decl) {
  Stream << "SingleImportDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const DemandImportDeclaration &Decl) {
  Stream << "DemandImportDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const ClassDeclaration &Decl) {
  Stream << "ClassDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const InterfaceDeclaration &Decl) {
  Stream << "InterfaceDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const FieldDeclaration &Decl) {
  Stream << "FieldDeclaration:" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const MethodDeclaration &Decl) {
  Stream << "MethodDeclaration:" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode()) << '\n';
  acceptChildrenLevel(Modifier);
}

void AST::PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName() << '\n';
}

void AST::PrintVisitor::acceptChildrenLevel(const AST::Node &Node) {
  for (const auto &Child : Node.getChildren()) {
    ++Level;
    Stream << std::string(Level * 2, ' ');
    Child->accept(*this);
    --Level;
  }
}
