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
void AST::Visitor::setError() { ErrorState = true; }

bool AST::Visitor::error() { return ErrorState; }

inline void AST::Visitor::dispatchChildren(const Node &Parent) {
  for (const auto &Child : Parent.getChildren()) {
    Child->accept(*this);
    if (ErrorState) {
      return;
    }
  }
}
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
  Stream << "FieldDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const ConstructorDeclaration &Decl) {
  Stream << "ConstructorDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const Extensions &Extensions) {
  Stream << "Extensions" << '\n';
  acceptChildrenLevel(Extensions);
}

void AST::PrintVisitor::visit(const Name &Name) {
  Stream << "Name" << '\n';
  acceptChildrenLevel(Name);
}

void AST::PrintVisitor::visit(const Identifier &Identifier) {
  Stream << "Identifier: " << Identifier.getName() << '\n';
}

void AST::PrintVisitor::visit(const MethodDeclaration &Decl) {
  Stream << "MethodDeclaration" << '\n';
  acceptChildrenLevel(Decl);
}

void AST::PrintVisitor::visit(const Modifier &Modifier) {
  Stream << "Modifier: " << ModifierCodeName.at(Modifier.getCode()) << '\n';
  acceptChildrenLevel(Modifier);
}

void AST::PrintVisitor::visit(const PrimitiveType &PrimitiveType) {
  Stream << "PrimitiveType: " << PrimitiveType.getType() << '\n';
}

void AST::PrintVisitor::visit(const SimpleType &SimpleType) {
  Stream << "SimpleType" << '\n';
  acceptChildrenLevel(SimpleType);
}

void AST::PrintVisitor::visit(const ArrayType &ArrayType) {
  Stream << "ArrayType" << '\n';
  acceptChildrenLevel(ArrayType);
}

void AST::PrintVisitor::visit(const VoidType &VoidType) {
  Stream << "VoidType: " << VoidType.getType()  << '\n';
}

void AST::PrintVisitor::visit(const Expression &Expression) {
  Stream << "Expression"  << '\n';
  acceptChildrenLevel(Expression);
}

void AST::PrintVisitor::visit(const CastExpression &CastExpression) {
  Stream << "CastExpression"  << '\n';
  acceptChildrenLevel(CastExpression);
}

void AST::PrintVisitor::visit(const SingleVariableDeclaration &SingleVariableDeclaration) {
  Stream << "SingleVariableDeclaration" << '\n';
  acceptChildrenLevel(SingleVariableDeclaration);
}

void AST::PrintVisitor::visit(const Super &Super) {
  Stream << "Super" << '\n';
  acceptChildrenLevel(Super);
}

void AST::PrintVisitor::visit(const Interfaces &Interfaces) {
  Stream << "Interfaces" << '\n';
  acceptChildrenLevel(Interfaces);
}

void AST::PrintVisitor::visit(const Block &Block) {
  Stream << "Block" << '\n';
  acceptChildrenLevel(Block);
}

void AST::PrintVisitor::visit(const IfThenStatement &IfThenStatement) {
  Stream << "IfThenStatement" << '\n';
  acceptChildrenLevel(IfThenStatement);
}

void AST::PrintVisitor::visit(const IfThenElseStatement &IfThenElseStatement) {
  Stream << "IfThenElseStatement" << '\n';
  acceptChildrenLevel(IfThenElseStatement);
}

void AST::PrintVisitor::visit(const WhileStatement &WhileStatement) {
  Stream << "WhileStatement" << '\n';
  acceptChildrenLevel(WhileStatement);
}

void AST::PrintVisitor::visit(const ForStatement &ForStatement) {
  Stream << "ForStatement" << '\n';
  acceptChildrenLevel(ForStatement);
}

void AST::PrintVisitor::visit(const SimpleStatement &SimpleStatement) {
  Stream << "SimpleStatement" << '\n';
  acceptChildrenLevel(SimpleStatement);
}

void AST::PrintVisitor::visit(const ClassInstanceCreation &ClassInstanceCreation) {
  Stream << "ClassInstanceCreation" << '\n';
  acceptChildrenLevel(ClassInstanceCreation);
}

void AST::PrintVisitor::visit(const OperationExpression &OperationExpression) {
  Stream << "OperationExpression" << '\n';
  acceptChildrenLevel(OperationExpression);
}

void AST::PrintVisitor::visit(const MethodInvocation &MethodInvocation) {
  Stream << "MethodInvocation" << '\n';
  acceptChildrenLevel(MethodInvocation);
}

void AST::PrintVisitor::visit(const ReturnStatement &ReturnStatement) {
  Stream << "ReturnStatement" << '\n';
  acceptChildrenLevel(ReturnStatement);
}

void AST::PrintVisitor::visit(const VariableDeclaration &VariableDeclaration) {
  Stream << "VariableDeclaration" << '\n';
  acceptChildrenLevel(VariableDeclaration);
}

void AST::PrintVisitor::visit(const AssignmentExpression &AssignmentExpression) {
  Stream << "AssignmentExpression" << '\n';
  acceptChildrenLevel(AssignmentExpression);
}

void AST::PrintVisitor::visit(const ASSIGN &ASSIGN) {
  Stream << "ASSIGN: " << ASSIGN.getSymbol() << '\n';
}

void AST::PrintVisitor::visit(const BinaryOperator &BinaryOperator) {
  Stream << "BinaryOperator: " << BinaryOperator.getBinaryOperatorSymbol() << '\n';
}

void AST::PrintVisitor::visit(const UnaryOperator &UnaryOperator) {
  Stream << "UnaryOperator: " << UnaryOperator.getUnaryOperatorSymbol() << '\n';
}

void AST::PrintVisitor::visit(const DecIntLiteral &DecIntLiteral) {
  Stream << "DecIntLiteral: " << DecIntLiteral.getLiteral() << '\n';
}

void AST::PrintVisitor::visit(const BooleanLiteral &BooleanLiteral) {
  Stream << "BooleanLiteral: " << BooleanLiteral.getLiteral() << '\n';
}

void AST::PrintVisitor::visit(const CharacterLiteral &CharacterLiteral) {
  Stream << "CharacterLiteral: " << CharacterLiteral.getLiteral() << '\n';
}

void AST::PrintVisitor::visit(const StringLiteral &StringLiteral) {
  Stream << "StringLiteral: " << StringLiteral.getLiteral() << '\n';
}

void AST::PrintVisitor::visit(const NullLiteral &NullLiteral) {
  Stream << "NullLiteral: " << NullLiteral.getLiteral() << '\n';
}

void AST::PrintVisitor::visit(const ArgumentList &ArgumentList) {
  Stream << "ArgumentList" << '\n';
  acceptChildrenLevel(ArgumentList);
}

void AST::PrintVisitor::visit(const FieldAccess &FieldAccess) {
  Stream << "FieldAccess" << '\n';
  acceptChildrenLevel(FieldAccess);
}

void AST::PrintVisitor::visit(const ArrayAccess &ArrayAccess) {
  Stream << "ArrayAccess" << '\n';
  acceptChildrenLevel(ArrayAccess);
}

void AST::PrintVisitor::visit(const ArrayCreation &ArrayCreation) {
  Stream << "ArrayCreation" << '\n';
  acceptChildrenLevel(ArrayCreation);
}

void AST::PrintVisitor::visit(const ThisExpression &ThisExpression) {
  Stream << "ThisExpression: " << ThisExpression.getExpression() << '\n';
}

void AST::PrintVisitor::visit(const ForInit &ForInit) {
  Stream << "ForInit" << '\n';
  acceptChildrenLevel(ForInit);
}

void AST::PrintVisitor::visit(const ForUpdate &ForUpdate) {
  Stream << "ForUpdate" << '\n';
  acceptChildrenLevel(ForUpdate);
}

void AST::PrintVisitor::acceptChildrenLevel(const AST::Node &Node) {
  for (const auto &Child : Node.getChildren()) {
    ++Level;
    Stream << std::string(Level * 2, ' ');
    Child->accept(*this);
    --Level;
  }
}
