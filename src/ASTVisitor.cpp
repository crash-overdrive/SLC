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

void AST::Visitor::setLog(std::ostream &Stream) {
  LogVisitorPtr = std::make_unique<LogVisitor>(Stream);
}

void AST::Visitor::setError() { ErrorState = true; }

bool AST::Visitor::error() { return ErrorState; }

inline void AST::Visitor::dispatchChildren(const Node &Parent) {
  if (!LogVisitorPtr) {
    LogVisitorPtr = std::make_unique<LogVisitor>();
  }
  Parent.accept(*LogVisitorPtr);
  LogVisitorPtr->log("\n");
  for (const auto &Child : Parent.getChildren()) {
    LogVisitorPtr->preVisit();
    Child->accept(*this);
    LogVisitorPtr->postVisit();
    if (ErrorState)
      return;
  }
}

AST::LogVisitor::LogVisitor() {}

AST::LogVisitor::LogVisitor(std::ostream &Stream) : Stream(Stream) {}

void AST::LogVisitor::visit(const Start &) { Stream << "Start"; }

void AST::LogVisitor::visit(const PackageDeclaration &) {
  Stream << "PackageDeclaration";
}

void AST::LogVisitor::visit(const SingleImportDeclaration &) {
  Stream << "SingleImportDeclaration";
}

void AST::LogVisitor::visit(const DemandImportDeclaration &) {
  Stream << "DemandImportDeclaration";
}

void AST::LogVisitor::visit(const ClassDeclaration &) {
  Stream << "ClassDeclaration";
}

void AST::LogVisitor::visit(const InterfaceDeclaration &) {
  Stream << "InterfaceDeclaration";
}

void AST::LogVisitor::visit(const FieldDeclaration &) {
  Stream << "FieldDeclaration";
}

void AST::LogVisitor::visit(const ConstructorDeclaration &) {
  Stream << "ConstructorDeclaration";
}

void AST::LogVisitor::visit(const Extensions &) { Stream << "Extensions"; }

void AST::LogVisitor::visit(const Name &) { Stream << "Name"; }

void AST::LogVisitor::visit(const MethodDeclaration &) {
  Stream << "MethodDeclaration";
}

void AST::LogVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode());
}

void AST::LogVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName();
}

void AST::LogVisitor::visit(const PrimitiveType &PrimitiveType) {
  Stream << "PrimitiveType: " << PrimitiveType.getType();
}

void AST::LogVisitor::visit(const SimpleType &) { Stream << "SimpleType"; }

void AST::LogVisitor::visit(const ArrayType &) { Stream << "ArrayType"; }

void AST::LogVisitor::visit(const VoidType &VoidType) {
  Stream << "VoidType: " << VoidType.getType();
}

void AST::LogVisitor::visit(const Expression &) { Stream << "Expression"; }

void AST::LogVisitor::visit(const CastExpression &) {
  Stream << "CastExpression";
}

void AST::LogVisitor::visit(const SingleVariableDeclaration &) {
  Stream << "SingleVariableDeclaration";
}

void AST::LogVisitor::visit(const Super &) { Stream << "Super"; }

void AST::LogVisitor::visit(const Interfaces &) { Stream << "Interfaces"; }

void AST::LogVisitor::visit(const Block &) { Stream << "Block"; }

void AST::LogVisitor::visit(const IfThenStatement &) {
  Stream << "IfThenStatement";
}

void AST::LogVisitor::visit(const IfThenElseStatement &) {
  Stream << "IfThenElseStatement";
}

void AST::LogVisitor::visit(const WhileStatement &) {
  Stream << "WhileStatement";
}

void AST::LogVisitor::visit(const ForStatement &) { Stream << "ForStatement"; }

void AST::LogVisitor::visit(const SimpleStatement &) {
  Stream << "SimpleStatement";
}

void AST::LogVisitor::visit(const ClassInstanceCreation &) {
  Stream << "ClassInstanceCreation";
}

void AST::LogVisitor::visit(const OperationExpression &) {
  Stream << "OperationExpression";
}

void AST::LogVisitor::visit(const MethodInvocation &) {
  Stream << "MethodInvocation";
}

void AST::LogVisitor::visit(const ReturnStatement &) {
  Stream << "ReturnStatement";
}

void AST::LogVisitor::visit(const VariableDeclaration &) {
  Stream << "VariableDeclaration";
}

void AST::LogVisitor::visit(const AssignmentExpression &) {
  Stream << "AssignmentExpression";
}

void AST::LogVisitor::visit(const ASSIGN &ASSIGN) {
  Stream << "ASSIGN: " << ASSIGN.getSymbol();
}

void AST::LogVisitor::visit(const BinaryOperator &BinaryOperator) {
  Stream << "BinaryOperator: " << BinaryOperator.getBinaryOperatorSymbol();
}

void AST::LogVisitor::visit(const UnaryOperator &UnaryOperator) {
  Stream << "UnaryOperator: " << UnaryOperator.getUnaryOperatorSymbol();
}

void AST::LogVisitor::visit(const DecIntLiteral &DecIntLiteral) {
  Stream << "DecIntLiteral: " << DecIntLiteral.getLiteral();
}

void AST::LogVisitor::visit(const BooleanLiteral &BooleanLiteral) {
  Stream << "BooleanLiteral: " << BooleanLiteral.getLiteral();
}

void AST::LogVisitor::visit(const CharacterLiteral &CharacterLiteral) {
  Stream << "CharacterLiteral: " << CharacterLiteral.getLiteral();
}

void AST::LogVisitor::visit(const StringLiteral &StringLiteral) {
  Stream << "StringLiteral: " << StringLiteral.getLiteral();
}

void AST::LogVisitor::visit(const NullLiteral &NullLiteral) {
  Stream << "NullLiteral: " << NullLiteral.getLiteral();
}

void AST::LogVisitor::visit(const ArgumentList &) { Stream << "ArgumentList"; }

void AST::LogVisitor::visit(const FieldAccess &) { Stream << "FieldAccess"; }

void AST::LogVisitor::visit(const ArrayAccess &) { Stream << "ArrayAccess"; }

void AST::LogVisitor::visit(const ArrayCreation &) {
  Stream << "ArrayCreation";
}

void AST::LogVisitor::visit(const ThisExpression &ThisExpression) {
  Stream << "ThisExpression: " << ThisExpression.getExpression();
}

void AST::LogVisitor::visit(const ForInit &) { Stream << "ForInit"; }

void AST::LogVisitor::visit(const ForUpdate &) { Stream << "ForUpdate"; }

void AST::LogVisitor::preVisit() {
  ++Level;
  Stream << std::string(Level * 2, ' ');
}

void AST::LogVisitor::postVisit() { --Level; }

void AST::LogVisitor::log(const std::string &Message) { Stream << Message; }
