#include "ASTVisitor.hpp"

namespace AST {

void Visitor::visit(const Start &node) { postVisit(node); }
void Visitor::visit(const InterfaceDeclaration &node) { postVisit(node); }
void Visitor::visit(const PackageDeclaration &node) { postVisit(node); }
void Visitor::visit(const SingleImportDeclaration &node) { postVisit(node); }
void Visitor::visit(const DemandImportDeclaration &node) { postVisit(node); }
void Visitor::visit(const ClassDeclaration &node) { postVisit(node); }
void Visitor::visit(const FieldDeclaration &node) { postVisit(node); }
void Visitor::visit(const ConstructorDeclaration &node) { postVisit(node); }
void Visitor::visit(const MethodDeclaration &node) { postVisit(node); }
void Visitor::visit(const Modifier &node) { postVisit(node); }
void Visitor::visit(const Identifier &node) { postVisit(node); }
void Visitor::visit(const Extensions &node) { postVisit(node); }
void Visitor::visit(const Name &node) { postVisit(node); }
void Visitor::visit(const PrimitiveType &node) { postVisit(node); }
void Visitor::visit(const SimpleType &node) { postVisit(node); }
void Visitor::visit(const ArrayType &node) { postVisit(node); }
void Visitor::visit(const VoidType &node) { postVisit(node); }
void Visitor::visit(const Expression &node) { postVisit(node); }
void Visitor::visit(const CastExpression &node) { postVisit(node); }
void Visitor::visit(const SingleVariableDeclaration &node) { postVisit(node); }
void Visitor::visit(const Super &node) { postVisit(node); }
void Visitor::visit(const Interfaces &node) { postVisit(node); }
void Visitor::visit(const Block &node) { postVisit(node); }
void Visitor::visit(const IfThenStatement &node) { postVisit(node); }
void Visitor::visit(const IfThenElseStatement &node) { postVisit(node); }
void Visitor::visit(const WhileStatement &node) { postVisit(node); }
void Visitor::visit(const ForStatement &node) { postVisit(node); }
void Visitor::visit(const SimpleStatement &node) { postVisit(node); }
void Visitor::visit(const ClassInstanceCreation &node) { postVisit(node); }
void Visitor::visit(const OperationExpression &node) { postVisit(node); }
void Visitor::visit(const MethodInvocation &node) { postVisit(node); }
void Visitor::visit(const ReturnStatement &node) { postVisit(node); }
void Visitor::visit(const VariableDeclaration &node) { postVisit(node); }
void Visitor::visit(const ASSIGN &node) { postVisit(node); }
void Visitor::visit(const AssignmentExpression &node) { postVisit(node); }
void Visitor::visit(const BinaryOperator &node) { postVisit(node); }
void Visitor::visit(const UnaryOperator &node) { postVisit(node); }
void Visitor::visit(const DecIntLiteral &node) { postVisit(node); }
void Visitor::visit(const CharacterLiteral &node) { postVisit(node); }
void Visitor::visit(const BooleanLiteral &node) { postVisit(node); }
void Visitor::visit(const StringLiteral &node) { postVisit(node); }
void Visitor::visit(const NullLiteral &node) { postVisit(node); }
void Visitor::visit(const ArgumentList &node) { postVisit(node); }
void Visitor::visit(const FieldAccess &node) { postVisit(node); }
void Visitor::visit(const ArrayAccess &node) { postVisit(node); }
void Visitor::visit(const ArrayCreation &node) { postVisit(node); }
void Visitor::visit(const ThisExpression &node) { postVisit(node); }
void Visitor::visit(const ForInit &node) { postVisit(node); }
void Visitor::visit(const ForUpdate &node) { postVisit(node); }

void Visitor::dispatchChildren(const Node &parent) {
  for (const auto &child : parent.getChildren()) {
    child->accept(*this);
  }
}

void Visitor::postVisit(const Node &) {}

PrintVisitor::PrintVisitor(std::ostream &stream) : stream(stream) {}

void PrintVisitor::visit(const Start &) { stream << "Start"; }

void PrintVisitor::visit(const PackageDeclaration &) {
  stream << "PackageDeclaration";
}

void PrintVisitor::visit(const SingleImportDeclaration &) {
  stream << "SingleImportDeclaration";
}

void PrintVisitor::visit(const DemandImportDeclaration &) {
  stream << "DemandImportDeclaration";
}

void PrintVisitor::visit(const ClassDeclaration &) {
  stream << "ClassDeclaration";
}

void PrintVisitor::visit(const InterfaceDeclaration &) {
  stream << "InterfaceDeclaration";
}

void PrintVisitor::visit(const FieldDeclaration &) {
  stream << "FieldDeclaration";
}

void PrintVisitor::visit(const ConstructorDeclaration &) {
  stream << "ConstructorDeclaration";
}

void PrintVisitor::visit(const Extensions &) { stream << "Extensions"; }

void PrintVisitor::visit(const Name &) { stream << "Name"; }

void PrintVisitor::visit(const MethodDeclaration &) {
  stream << "MethodDeclaration";
}

void PrintVisitor::visit(const Modifier &modifier) {
  stream << "Modifier: " << modifierCodeName.at(modifier.getCode());
}

void PrintVisitor::visit(const Identifier &identifier) {
  stream << "Identifier: " << identifier.getName();
}

void PrintVisitor::visit(const PrimitiveType &primitiveType) {
  stream << "PrimitiveType: " << primitiveType.getType();
}

void PrintVisitor::visit(const SimpleType &) { stream << "SimpleType"; }

void PrintVisitor::visit(const ArrayType &) { stream << "ArrayType"; }

void PrintVisitor::visit(const VoidType &voidType) {
  stream << "VoidType: " << voidType.getType();
}

void PrintVisitor::visit(const Expression &) { stream << "Expression"; }

void PrintVisitor::visit(const CastExpression &) { stream << "CastExpression"; }

void PrintVisitor::visit(const SingleVariableDeclaration &) {
  stream << "SingleVariableDeclaration";
}

void PrintVisitor::visit(const Super &) { stream << "Super"; }

void PrintVisitor::visit(const Interfaces &) { stream << "Interfaces"; }

void PrintVisitor::visit(const Block &) { stream << "Block"; }

void PrintVisitor::visit(const IfThenStatement &) {
  stream << "IfThenStatement";
}

void PrintVisitor::visit(const IfThenElseStatement &) {
  stream << "IfThenElseStatement";
}

void PrintVisitor::visit(const WhileStatement &) { stream << "WhileStatement"; }

void PrintVisitor::visit(const ForStatement &) { stream << "ForStatement"; }

void PrintVisitor::visit(const SimpleStatement &) {
  stream << "SimpleStatement";
}

void PrintVisitor::visit(const ClassInstanceCreation &) {
  stream << "ClassInstanceCreation";
}

void PrintVisitor::visit(const OperationExpression &) {
  stream << "OperationExpression";
}

void PrintVisitor::visit(const MethodInvocation &) {
  stream << "MethodInvocation";
}

void PrintVisitor::visit(const ReturnStatement &) {
  stream << "ReturnStatement";
}

void PrintVisitor::visit(const VariableDeclaration &) {
  stream << "VariableDeclaration";
}

void PrintVisitor::visit(const AssignmentExpression &) {
  stream << "AssignmentExpression";
}

void PrintVisitor::visit(const ASSIGN &assign) {
  stream << "ASSIGN: " << assign.getSymbol();
}

void PrintVisitor::visit(const BinaryOperator &binaryOperator) {
  stream << "BinaryOperator: " << binaryOperator.getBinaryOperatorSymbol();
}

void PrintVisitor::visit(const UnaryOperator &unaryOperator) {
  stream << "UnaryOperator: " << unaryOperator.getUnaryOperatorSymbol();
}

void PrintVisitor::visit(const DecIntLiteral &decIntLiteral) {
  stream << "DecIntLiteral: " << decIntLiteral.getLiteral();
}

void PrintVisitor::visit(const BooleanLiteral &booleanLiteral) {
  stream << "BooleanLiteral: " << booleanLiteral.getLiteral();
}

void PrintVisitor::visit(const CharacterLiteral &characterLiteral) {
  stream << "CharacterLiteral: " << characterLiteral.getLiteral();
}

void PrintVisitor::visit(const StringLiteral &stringLiteral) {
  stream << "StringLiteral: " << stringLiteral.getLiteral();
}

void PrintVisitor::visit(const NullLiteral &nullLiteral) {
  stream << "NullLiteral: " << nullLiteral.getLiteral();
}

void PrintVisitor::visit(const ArgumentList &) { stream << "ArgumentList"; }

void PrintVisitor::visit(const FieldAccess &) { stream << "FieldAccess"; }

void PrintVisitor::visit(const ArrayAccess &) { stream << "ArrayAccess"; }

void PrintVisitor::visit(const ArrayCreation &) { stream << "ArrayCreation"; }

void PrintVisitor::visit(const ThisExpression &thisExpression) {
  stream << "ThisExpression: " << thisExpression.getExpression();
}

void PrintVisitor::visit(const ForInit &) { stream << "ForInit"; }

void PrintVisitor::visit(const ForUpdate &) { stream << "ForUpdate"; }

void TrackVisitor::setLog(std::ostream &stream) { this->streamRef = stream; }

void TrackVisitor::setError() { errorState = true; }

bool TrackVisitor::isErrorState() { return errorState; }

inline void TrackVisitor::postVisit(const Node &parent) {
  streamRef.get() << parent << '\n';
  for (const auto &child : parent.getChildren()) {
    level++;
    streamRef.get() << std::string(level * 2, ' ');
    child->accept(*this);
    level--;
    if (errorState)
      return;
  }
}

} // namespace AST
