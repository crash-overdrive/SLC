#include "ASTVisitor.hpp"

namespace AST {

void Visitor::visit(const Start &Node) { postVisit(Node); }

void Visitor::visit(const InterfaceDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const PackageDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const SingleImportDeclaration &Node) {
  postVisit(Node);
}
void Visitor::visit(const DemandImportDeclaration &Node) {
  postVisit(Node);
}
void Visitor::visit(const ClassDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const FieldDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const ConstructorDeclaration &Node) {
  postVisit(Node);
}
void Visitor::visit(const MethodDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const Modifier &Node) { postVisit(Node); }
void Visitor::visit(const Identifier &Node) { postVisit(Node); }
void Visitor::visit(const Extensions &Node) { postVisit(Node); }
void Visitor::visit(const Name &Node) { postVisit(Node); }
void Visitor::visit(const PrimitiveType &Node) { postVisit(Node); }
void Visitor::visit(const SimpleType &Node) { postVisit(Node); }
void Visitor::visit(const ArrayType &Node) { postVisit(Node); }
void Visitor::visit(const VoidType &Node) { postVisit(Node); }
void Visitor::visit(const Expression &Node) { postVisit(Node); }
void Visitor::visit(const CastExpression &Node) { postVisit(Node); }
void Visitor::visit(const SingleVariableDeclaration &Node) {
  postVisit(Node);
}
void Visitor::visit(const Super &Node) { postVisit(Node); }
void Visitor::visit(const Interfaces &Node) { postVisit(Node); }
void Visitor::visit(const Block &Node) { postVisit(Node); }
void Visitor::visit(const IfThenStatement &Node) { postVisit(Node); }
void Visitor::visit(const IfThenElseStatement &Node) { postVisit(Node); }
void Visitor::visit(const WhileStatement &Node) { postVisit(Node); }
void Visitor::visit(const ForStatement &Node) { postVisit(Node); }
void Visitor::visit(const SimpleStatement &Node) { postVisit(Node); }
void Visitor::visit(const ClassInstanceCreation &Node) { postVisit(Node); }
void Visitor::visit(const OperationExpression &Node) { postVisit(Node); }
void Visitor::visit(const MethodInvocation &Node) { postVisit(Node); }
void Visitor::visit(const ReturnStatement &Node) { postVisit(Node); }
void Visitor::visit(const VariableDeclaration &Node) { postVisit(Node); }
void Visitor::visit(const ASSIGN &Node) { postVisit(Node); }
void Visitor::visit(const AssignmentExpression &Node) { postVisit(Node); }
void Visitor::visit(const BinaryOperator &Node) { postVisit(Node); }
void Visitor::visit(const UnaryOperator &Node) { postVisit(Node); }
void Visitor::visit(const DecIntLiteral &Node) { postVisit(Node); }
void Visitor::visit(const CharacterLiteral &Node) { postVisit(Node); }
void Visitor::visit(const BooleanLiteral &Node) { postVisit(Node); }
void Visitor::visit(const StringLiteral &Node) { postVisit(Node); }
void Visitor::visit(const NullLiteral &Node) { postVisit(Node); }
void Visitor::visit(const ArgumentList &Node) { postVisit(Node); }
void Visitor::visit(const FieldAccess &Node) { postVisit(Node); }
void Visitor::visit(const ArrayAccess &Node) { postVisit(Node); }
void Visitor::visit(const ArrayCreation &Node) { postVisit(Node); }
void Visitor::visit(const ThisExpression &Node) { postVisit(Node); }
void Visitor::visit(const ForInit &Node) { postVisit(Node); }
void Visitor::visit(const ForUpdate &Node) { postVisit(Node); }

void Visitor::postVisit(const Node &) {}

void Visitor::dispatchChildren(const Node &Parent) {
  for (const auto &Child : Parent.getChildren()) {
    Child->accept(*this);
  }
}

PrintVisitor::PrintVisitor(std::ostream &Stream) : Stream(Stream) {}

void PrintVisitor::visit(const Start &) { Stream << "Start"; }

void PrintVisitor::visit(const PackageDeclaration &) {
  Stream << "PackageDeclaration";
}

void PrintVisitor::visit(const SingleImportDeclaration &) {
  Stream << "SingleImportDeclaration";
}

void PrintVisitor::visit(const DemandImportDeclaration &) {
  Stream << "DemandImportDeclaration";
}

void PrintVisitor::visit(const ClassDeclaration &) {
  Stream << "ClassDeclaration";
}

void PrintVisitor::visit(const InterfaceDeclaration &) {
  Stream << "InterfaceDeclaration";
}

void PrintVisitor::visit(const FieldDeclaration &) {
  Stream << "FieldDeclaration";
}

void PrintVisitor::visit(const ConstructorDeclaration &) {
  Stream << "ConstructorDeclaration";
}

void PrintVisitor::visit(const Extensions &) { Stream << "Extensions"; }

void PrintVisitor::visit(const Name &) { Stream << "Name"; }

void PrintVisitor::visit(const MethodDeclaration &) {
  Stream << "MethodDeclaration";
}

void PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode());
}

void PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName();
}

void PrintVisitor::visit(const PrimitiveType &PrimitiveType) {
  Stream << "PrimitiveType: " << PrimitiveType.getType();
}

void PrintVisitor::visit(const SimpleType &) { Stream << "SimpleType"; }

void PrintVisitor::visit(const ArrayType &) { Stream << "ArrayType"; }

void PrintVisitor::visit(const VoidType &VoidType) {
  Stream << "VoidType: " << VoidType.getType();
}

void PrintVisitor::visit(const Expression &) { Stream << "Expression"; }

void PrintVisitor::visit(const CastExpression &) {
  Stream << "CastExpression";
}

void PrintVisitor::visit(const SingleVariableDeclaration &) {
  Stream << "SingleVariableDeclaration";
}

void PrintVisitor::visit(const Super &) { Stream << "Super"; }

void PrintVisitor::visit(const Interfaces &) { Stream << "Interfaces"; }

void PrintVisitor::visit(const Block &) { Stream << "Block"; }

void PrintVisitor::visit(const IfThenStatement &) {
  Stream << "IfThenStatement";
}

void PrintVisitor::visit(const IfThenElseStatement &) {
  Stream << "IfThenElseStatement";
}

void PrintVisitor::visit(const WhileStatement &) {
  Stream << "WhileStatement";
}

void PrintVisitor::visit(const ForStatement &) {
  Stream << "ForStatement";
}

void PrintVisitor::visit(const SimpleStatement &) {
  Stream << "SimpleStatement";
}

void PrintVisitor::visit(const ClassInstanceCreation &) {
  Stream << "ClassInstanceCreation";
}

void PrintVisitor::visit(const OperationExpression &) {
  Stream << "OperationExpression";
}

void PrintVisitor::visit(const MethodInvocation &) {
  Stream << "MethodInvocation";
}

void PrintVisitor::visit(const ReturnStatement &) {
  Stream << "ReturnStatement";
}

void PrintVisitor::visit(const VariableDeclaration &) {
  Stream << "VariableDeclaration";
}

void PrintVisitor::visit(const AssignmentExpression &) {
  Stream << "AssignmentExpression";
}

void PrintVisitor::visit(const ASSIGN &ASSIGN) {
  Stream << "ASSIGN: " << ASSIGN.getSymbol();
}

void PrintVisitor::visit(const BinaryOperator &BinaryOperator) {
  Stream << "BinaryOperator: " << BinaryOperator.getBinaryOperatorSymbol();
}

void PrintVisitor::visit(const UnaryOperator &UnaryOperator) {
  Stream << "UnaryOperator: " << UnaryOperator.getUnaryOperatorSymbol();
}

void PrintVisitor::visit(const DecIntLiteral &DecIntLiteral) {
  Stream << "DecIntLiteral: " << DecIntLiteral.getLiteral();
}

void PrintVisitor::visit(const BooleanLiteral &BooleanLiteral) {
  Stream << "BooleanLiteral: " << BooleanLiteral.getLiteral();
}

void PrintVisitor::visit(const CharacterLiteral &CharacterLiteral) {
  Stream << "CharacterLiteral: " << CharacterLiteral.getLiteral();
}

void PrintVisitor::visit(const StringLiteral &StringLiteral) {
  Stream << "StringLiteral: " << StringLiteral.getLiteral();
}

void PrintVisitor::visit(const NullLiteral &NullLiteral) {
  Stream << "NullLiteral: " << NullLiteral.getLiteral();
}

void PrintVisitor::visit(const ArgumentList &) {
  Stream << "ArgumentList";
}

void PrintVisitor::visit(const FieldAccess &) { Stream << "FieldAccess"; }

void PrintVisitor::visit(const ArrayAccess &) { Stream << "ArrayAccess"; }

void PrintVisitor::visit(const ArrayCreation &) {
  Stream << "ArrayCreation";
}

void PrintVisitor::visit(const ThisExpression &ThisExpression) {
  Stream << "ThisExpression: " << ThisExpression.getExpression();
}

void PrintVisitor::visit(const ForInit &) { Stream << "ForInit"; }

void PrintVisitor::visit(const ForUpdate &) { Stream << "ForUpdate"; }

void TrackVisitor::setLog(std::ostream &Stream) {
  this->StreamRef = Stream;
}

void TrackVisitor::setError() { ErrorState = true; }

bool TrackVisitor::error() { return ErrorState; }

inline void TrackVisitor::postVisit(const Node &Parent) {
  StreamRef.get() << Parent << '\n';
  for (const auto &Child : Parent.getChildren()) {
    Level++;
    StreamRef.get() << std::string(Level * 2, ' ');
    Child->accept(*this);
    Level--;
    if (ErrorState)
      return;
  }
}

} // namespace AST
