#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

namespace AST {

void Node::addChild(std::unique_ptr<Node> child) {
  children.emplace_back(std::move(child));
}

const std::vector<std::unique_ptr<Node>> &Node::getChildren() const {
  return children;
}

void Start::accept(Visitor &visitor) const { visitor.visit(*this); }

void PackageDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void SingleImportDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void DemandImportDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void ClassDeclaration::accept(Visitor &visitor) const { visitor.visit(*this); }

void InterfaceDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void MethodDeclaration::accept(Visitor &visitor) const { visitor.visit(*this); }

void FieldDeclaration::accept(Visitor &visitor) const { visitor.visit(*this); }

void ConstructorDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void Extensions::accept(Visitor &visitor) const { visitor.visit(*this); }

void Name::accept(Visitor &visitor) const { visitor.visit(*this); }

void ArrayType::accept(Visitor &visitor) const { visitor.visit(*this); }

void SimpleType::accept(Visitor &visitor) const { visitor.visit(*this); }

void Expression::accept(Visitor &visitor) const { visitor.visit(*this); }

void CastExpression::accept(Visitor &visitor) const { visitor.visit(*this); }

void SingleVariableDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void Super::accept(Visitor &visitor) const { visitor.visit(*this); }

void Interfaces::accept(Visitor &visitor) const { visitor.visit(*this); }

void Block::accept(Visitor &visitor) const { visitor.visit(*this); }

void IfThenStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

void IfThenElseStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void WhileStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

void ForStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

void ForInit::accept(Visitor &visitor) const { visitor.visit(*this); }

void ForUpdate::accept(Visitor &visitor) const { visitor.visit(*this); }

void ExpressionStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void VariableDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void ReturnStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

void ClassInstanceCreation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AssignmentExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void BinaryExpression::accept(Visitor &visitor) const { visitor.visit(*this); }

void InstanceOfExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

Operator::Operator(const std::string &operatorSymbol)
    : operatorSymbol(operatorSymbol) {}

const std::string &Operator::getOperatorSymbol() const {
  return operatorSymbol;
}

void Operator::accept(Visitor &visitor) const { visitor.visit(*this); }

void MethodNameInvocation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void MethodPrimaryInvocation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void FieldAccess::accept(Visitor &visitor) const { visitor.visit(*this); }

void ArrayAccess::accept(Visitor &visitor) const { visitor.visit(*this); }

void ArrayCreation::accept(Visitor &visitor) const { visitor.visit(*this); }

void UnaryExpression::accept(Visitor &visitor) const { visitor.visit(*this); }

void ThisExpression::accept(Visitor &visitor) const { visitor.visit(*this); }

void ArgumentList::accept(Visitor &visitor) const { visitor.visit(*this); }

Modifier::Modifier(std::string name) : name(std::move(name)) {}

const std::string &Modifier::getName() const { return name; }

void Modifier::accept(Visitor &visitor) const { visitor.visit(*this); }

Identifier::Identifier(const std::string &name) : name(name) {}

const std::string &Identifier::getName() const { return name; }

void Identifier::accept(Visitor &visitor) const { visitor.visit(*this); }

PrimitiveType::PrimitiveType(const std::string &type) : type(type) {}

const std::string &PrimitiveType::getType() const { return type; }

void PrimitiveType::accept(Visitor &visitor) const { visitor.visit(*this); }

DecIntLiteral::DecIntLiteral(const std::string &literal) : literal(literal) {}

const std::string &DecIntLiteral::getLiteral() const { return literal; }

void DecIntLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

BooleanLiteral::BooleanLiteral(const std::string &literal) : literal(literal) {}

const std::string &BooleanLiteral::getLiteral() const { return literal; }

void BooleanLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

CharacterLiteral::CharacterLiteral(const std::string &literal)
    : literal(literal) {}

const std::string &CharacterLiteral::getLiteral() const { return literal; }

void CharacterLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

StringLiteral::StringLiteral(const std::string &literal) : literal(literal) {}

const std::string &StringLiteral::getLiteral() const { return literal; }

void StringLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

NullLiteral::NullLiteral(const std::string &literal) : literal(literal) {}

const std::string &NullLiteral::getLiteral() const { return literal; }

void NullLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

std::ostream &operator<<(std::ostream &stream, const Node &node) {
  PrintVisitor visitor(stream);
  node.accept(visitor);
  return stream;
}

} // namespace AST
