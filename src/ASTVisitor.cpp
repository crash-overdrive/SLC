#include "ASTVisitor.hpp"

void AST::Visitor::visit(const Start &) {}
void AST::Visitor::visit(const InterfaceDeclaration &) {}
void AST::Visitor::visit(const PackageDeclaration &) {}
void AST::Visitor::visit(const SingleImportDeclaration &) {}
void AST::Visitor::visit(const DemandImportDeclaration &) {}
void AST::Visitor::visit(const ClassDeclaration &) {}
void AST::Visitor::visit(const FieldDeclaration &) {}
void AST::Visitor::visit(const ConstructorDeclaration &) {}
void AST::Visitor::visit(const MethodDeclaration &) {}
void AST::Visitor::visit(const Modifier &) {}
void AST::Visitor::visit(const Identifier &) {}
void AST::Visitor::visit(const Extensions &) {}
void AST::Visitor::visit(const Name &) {}
void AST::Visitor::visit(const PrimitiveType &) {}
void AST::Visitor::visit(const SimpleType &) {}
void AST::Visitor::visit(const ArrayType &) {}
void AST::Visitor::visit(const VoidType &) {}
void AST::Visitor::visit(const FormalParameterList &) {}
void AST::Visitor::visit(const SingleVariableDeclaration &) {}
void AST::Visitor::visit(const Super &) {}
void AST::Visitor::visit(const Interfaces &) {}

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

void AST::PrintVisitor::visit(const ConstructorDeclaration &Decl) {
  Stream << "ConstructorDeclaration:" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const Extensions &Extensions) {
  Stream << "Extensions:" << '\n';
  acceptChildrenLevel(Extensions);
}

void AST::PrintVisitor::visit(const Name &Name) {
  Stream << "Name:" << '\n';
  acceptChildrenLevel(Name);
}

void AST::PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName() << '\n';
}

void AST::PrintVisitor::visit(const MethodDeclaration &Decl) {
  Stream << "MethodDeclaration:" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode()) << '\n';
  acceptChildrenLevel(Modifier);
}

void AST::PrintVisitor::visit(const PrimitiveType &PrimitiveType) {
  Stream << "PrimitiveType: " << PrimitiveType.getType() << "\n";
}

void AST::PrintVisitor::visit(const SimpleType &SimpleType) {
  Stream << "SimpleType: " << "\n";
  acceptChildrenLevel(SimpleType);
}

void AST::PrintVisitor::visit(const ArrayType &ArrayType) {
  Stream << "ArrayType: " << "\n";
  acceptChildrenLevel(ArrayType);
}

void AST::PrintVisitor::visit(const VoidType &VoidType) {
  Stream << "VoidType: " << VoidType.getType()  << "\n";
}

void AST::PrintVisitor::visit(const FormalParameterList &FormalParameterList) {
  Stream << "FormalParameterList: "  << "\n";
  acceptChildrenLevel(FormalParameterList);
}

void AST::PrintVisitor::visit(const SingleVariableDeclaration &SingleVariableDeclaration) {
  Stream << "SingleVariableDeclaration: " << "\n";
  acceptChildrenLevel(SingleVariableDeclaration);
}

void AST::PrintVisitor::visit(const Super &Super) {
  Stream << "Super: " << "\n";
  acceptChildrenLevel(Super);
}

void AST::PrintVisitor::visit(const Interfaces &Interfaces) {
  Stream << "Interfaces: " << "\n";
  acceptChildrenLevel(Interfaces);
}

void AST::PrintVisitor::acceptChildrenLevel(const AST::Node &Node) {
  for (const auto &Child : Node.getChildren()) {
    ++Level;
    Stream << std::string(Level * 2, ' ');
    Child->accept(*this);
    --Level;
  }
}
