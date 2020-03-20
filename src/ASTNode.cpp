#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

void AST::Node::addChild(std::unique_ptr<Node> child) {
  children.emplace_back(std::move(child));
}

const std::vector<std::unique_ptr<AST::Node>> &AST::Node::getChildren() const {
  return children;
}

void AST::Start::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::PackageDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::SingleImportDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::DemandImportDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ClassDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::InterfaceDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::MethodDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::FieldDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ConstructorDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::Extensions::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::Name::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::ArrayType::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::SimpleType::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::Expression::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::CastExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::SingleVariableDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::Super::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::Interfaces::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::Block::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::IfThenStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::IfThenElseStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::WhileStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ForStatement::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::SimpleStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ClassInstanceCreation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::OperationExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::MethodInvocation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ReturnStatement::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::VariableDeclaration::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::AssignmentExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ArgumentList::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::FieldAccess::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::ArrayAccess::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::ArrayCreation::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

void AST::ForInit::accept(Visitor &visitor) const { visitor.visit(*this); }

void AST::ForUpdate::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::Modifier::Modifier(std::string name) : name(std::move(name)) {}

const std::string &AST::Modifier::getName() const { return name; }

void AST::Modifier::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::Identifier::Identifier(const std::string &name) : name(name) {}

const std::string &AST::Identifier::getName() const { return name; }

void AST::Identifier::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::PrimitiveType::PrimitiveType(const std::string &type) : type(type) {}

const std::string &AST::PrimitiveType::getType() const { return type; }

void AST::PrimitiveType::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::VoidType::VoidType(const std::string &type) : type(type) {}

const std::string &AST::VoidType::getType() const { return type; }

void AST::VoidType::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::ASSIGN::ASSIGN(const std::string &symbol) : symbol(symbol) {}

const std::string &AST::ASSIGN::getSymbol() const { return symbol; }

void AST::ASSIGN::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::BinaryOperator::BinaryOperator(const std::string &binaryOperatorSymbol)
    : binaryOperatorSymbol(binaryOperatorSymbol) {}

const std::string &AST::BinaryOperator::getBinaryOperatorSymbol() const {
  return binaryOperatorSymbol;
}

void AST::BinaryOperator::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::UnaryOperator::UnaryOperator(const std::string &unaryOperatorSymbol)
    : unaryOperatorSymbol(unaryOperatorSymbol) {}

const std::string &AST::UnaryOperator::getUnaryOperatorSymbol() const {
  return unaryOperatorSymbol;
}

void AST::UnaryOperator::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::DecIntLiteral::DecIntLiteral(const std::string &literal)
    : literal(literal) {}

const std::string &AST::DecIntLiteral::getLiteral() const { return literal; }

void AST::DecIntLiteral::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::BooleanLiteral::BooleanLiteral(const std::string &literal)
    : literal(literal) {}

const std::string &AST::BooleanLiteral::getLiteral() const { return literal; }

void AST::BooleanLiteral::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::CharacterLiteral::CharacterLiteral(const std::string &literal)
    : literal(literal) {}

const std::string &AST::CharacterLiteral::getLiteral() const { return literal; }

void AST::CharacterLiteral::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::StringLiteral::StringLiteral(const std::string &literal)
    : literal(literal) {}

const std::string &AST::StringLiteral::getLiteral() const { return literal; }

void AST::StringLiteral::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

AST::NullLiteral::NullLiteral(const std::string &literal) : literal(literal) {}

const std::string &AST::NullLiteral::getLiteral() const { return literal; }

void AST::NullLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }

AST::ThisExpression::ThisExpression(const std::string &expression)
    : expression(expression) {}

const std::string &AST::ThisExpression::getExpression() const {
  return expression;
}

void AST::ThisExpression::accept(Visitor &visitor) const {
  visitor.visit(*this);
}

std::ostream &AST::operator<<(std::ostream &stream, const Node &node) {
  PrintVisitor visitor(stream);
  node.accept(visitor);
  return stream;
}
