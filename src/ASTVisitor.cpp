#include "ASTVisitor.hpp"

void AST::Visitor::visit(const Start &Node) { dispatchChildren(Node); }

void AST::Visitor::visit(const InterfaceDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const PackageDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const SingleImportDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const DemandImportDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const ClassDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const FieldDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const ConstructorDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const MethodDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const Modifier &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Identifier &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Extensions &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Name &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const PrimitiveType &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const SimpleType &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ArrayType &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const VoidType &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Expression &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const CastExpression &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const SingleVariableDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const Super &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Interfaces &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const Block &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const IfThenStatement &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const IfThenElseStatement &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const WhileStatement &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ForStatement &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const SimpleStatement &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const ClassInstanceCreation &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const OperationExpression &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const MethodInvocation &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const ReturnStatement &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const VariableDeclaration &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const ASSIGN &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const AssignmentExpression &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const BinaryOperator &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const UnaryOperator &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const DecIntLiteral &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const CharacterLiteral &Node) {
  dispatchChildren(Node);
}
void AST::Visitor::visit(const BooleanLiteral &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const StringLiteral &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const NullLiteral &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ArgumentList &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const FieldAccess &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ArrayAccess &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ArrayCreation &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ThisExpression &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ForInit &Node) { dispatchChildren(Node); }
void AST::Visitor::visit(const ForUpdate &Node) { dispatchChildren(Node); }

inline void AST::Visitor::dispatchChildren(const Node &Parent) {
  for (const auto &Child : Parent.getChildren()) {
    Child->accept(*this);
  }
}

AST::PrintVisitor::PrintVisitor() {}

AST::PrintVisitor::PrintVisitor(std::ostream &Stream) : Stream(Stream) {}

void AST::PrintVisitor::visit(const Start &) { Stream << "Start"; }

void AST::PrintVisitor::visit(const PackageDeclaration &) {
  Stream << "PackageDeclaration";
}

void AST::PrintVisitor::visit(const SingleImportDeclaration &) {
  Stream << "SingleImportDeclaration";
}

void AST::PrintVisitor::visit(const DemandImportDeclaration &) {
  Stream << "DemandImportDeclaration";
}

void AST::PrintVisitor::visit(const ClassDeclaration &) {
  Stream << "ClassDeclaration";
}

void AST::PrintVisitor::visit(const InterfaceDeclaration &) {
  Stream << "InterfaceDeclaration";
}

void AST::PrintVisitor::visit(const FieldDeclaration &) {
  Stream << "FieldDeclaration";
}

void AST::PrintVisitor::visit(const ConstructorDeclaration &) {
  Stream << "ConstructorDeclaration";
}

void AST::PrintVisitor::visit(const Extensions &) { Stream << "Extensions"; }

void AST::PrintVisitor::visit(const Name &) { Stream << "Name"; }

void AST::PrintVisitor::visit(const MethodDeclaration &) {
  Stream << "MethodDeclaration";
}

void AST::PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode());
}

void AST::PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName();
}

void AST::PrintVisitor::visit(const PrimitiveType &PrimitiveType) {
  Stream << "PrimitiveType: " << PrimitiveType.getType();
}

void AST::PrintVisitor::visit(const SimpleType &) { Stream << "SimpleType"; }

void AST::PrintVisitor::visit(const ArrayType &) { Stream << "ArrayType"; }

void AST::PrintVisitor::visit(const VoidType &VoidType) {
  Stream << "VoidType: " << VoidType.getType();
}

void AST::PrintVisitor::visit(const Expression &) { Stream << "Expression"; }

void AST::PrintVisitor::visit(const CastExpression &) {
  Stream << "CastExpression";
}

void AST::PrintVisitor::visit(const SingleVariableDeclaration &) {
  Stream << "SingleVariableDeclaration";
}

void AST::PrintVisitor::visit(const Super &) { Stream << "Super"; }

void AST::PrintVisitor::visit(const Interfaces &) { Stream << "Interfaces"; }

void AST::PrintVisitor::visit(const Block &) { Stream << "Block"; }

void AST::PrintVisitor::visit(const IfThenStatement &) {
  Stream << "IfThenStatement";
}

void AST::PrintVisitor::visit(const IfThenElseStatement &) {
  Stream << "IfThenElseStatement";
}

void AST::PrintVisitor::visit(const WhileStatement &) {
  Stream << "WhileStatement";
}

void AST::PrintVisitor::visit(const ForStatement &) {
  Stream << "ForStatement";
}

void AST::PrintVisitor::visit(const SimpleStatement &) {
  Stream << "SimpleStatement";
}

void AST::PrintVisitor::visit(const ClassInstanceCreation &) {
  Stream << "ClassInstanceCreation";
}

void AST::PrintVisitor::visit(const OperationExpression &) {
  Stream << "OperationExpression";
}

void AST::PrintVisitor::visit(const MethodInvocation &) {
  Stream << "MethodInvocation";
}

void AST::PrintVisitor::visit(const ReturnStatement &) {
  Stream << "ReturnStatement";
}

void AST::PrintVisitor::visit(const VariableDeclaration &) {
  Stream << "VariableDeclaration";
}

void AST::PrintVisitor::visit(const AssignmentExpression &) {
  Stream << "AssignmentExpression";
}

void AST::PrintVisitor::visit(const ASSIGN &ASSIGN) {
  Stream << "ASSIGN: " << ASSIGN.getSymbol();
}

void AST::PrintVisitor::visit(const BinaryOperator &BinaryOperator) {
  Stream << "BinaryOperator: " << BinaryOperator.getBinaryOperatorSymbol();
}

void AST::PrintVisitor::visit(const UnaryOperator &UnaryOperator) {
  Stream << "UnaryOperator: " << UnaryOperator.getUnaryOperatorSymbol();
}

void AST::PrintVisitor::visit(const DecIntLiteral &DecIntLiteral) {
  Stream << "DecIntLiteral: " << DecIntLiteral.getLiteral();
}

void AST::PrintVisitor::visit(const BooleanLiteral &BooleanLiteral) {
  Stream << "BooleanLiteral: " << BooleanLiteral.getLiteral();
}

void AST::PrintVisitor::visit(const CharacterLiteral &CharacterLiteral) {
  Stream << "CharacterLiteral: " << CharacterLiteral.getLiteral();
}

void AST::PrintVisitor::visit(const StringLiteral &StringLiteral) {
  Stream << "StringLiteral: " << StringLiteral.getLiteral();
}

void AST::PrintVisitor::visit(const NullLiteral &NullLiteral) {
  Stream << "NullLiteral: " << NullLiteral.getLiteral();
}

void AST::PrintVisitor::visit(const ArgumentList &) {
  Stream << "ArgumentList";
}

void AST::PrintVisitor::visit(const FieldAccess &) { Stream << "FieldAccess"; }

void AST::PrintVisitor::visit(const ArrayAccess &) { Stream << "ArrayAccess"; }

void AST::PrintVisitor::visit(const ArrayCreation &) {
  Stream << "ArrayCreation";
}

void AST::PrintVisitor::visit(const ThisExpression &ThisExpression) {
  Stream << "ThisExpression: " << ThisExpression.getExpression();
}

void AST::PrintVisitor::visit(const ForInit &) { Stream << "ForInit"; }

void AST::PrintVisitor::visit(const ForUpdate &) { Stream << "ForUpdate"; }

void AST::PrintVisitor::preVisit() {
  ++Level;
  Stream << std::string(Level * 2, ' ');
}

void AST::PrintVisitor::postVisit() { --Level; }

void AST::PrintVisitor::print(const std::string &Message) { Stream << Message; }

void AST::TrackVisitor::setLog(std::ostream &Stream) {
  PrintVisitorPtr = std::make_unique<PrintVisitor>(Stream);
}

void AST::TrackVisitor::setError() { ErrorState = true; }

bool AST::TrackVisitor::error() { return ErrorState; }

inline void AST::TrackVisitor::dispatchChildren(const Node &Parent) {
  Parent.accept(*PrintVisitorPtr);
  PrintVisitorPtr->print("\n");
  for (const auto &Child : Parent.getChildren()) {
    PrintVisitorPtr->preVisit();
    Child->accept(*this);
    PrintVisitorPtr->postVisit();
    if (ErrorState)
      return;
  }
}
