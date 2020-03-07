#include <ASTNode.hpp>
#include <ASTVisitor.hpp>

void AST::Node::addChild(std::unique_ptr<Node> Child) {
  Children.emplace_back(std::move(Child));
}

const std::vector<std::unique_ptr<AST::Node>> &AST::Node::getChildren() const {
  return Children;
}

void AST::Start::accept(Visitor &Visitor) const { Visitor.visit(*this); }

void AST::PackageDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::SingleImportDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::DemandImportDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ClassDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::InterfaceDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::MethodDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::FieldDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ConstructorDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::Extensions::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::Name::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ArrayType::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::SimpleType::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::FormalParameterList::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::SingleVariableDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::Super::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::Interfaces::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::Block::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::IfThenStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::IfThenElseStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::WhileStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ForStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::SimpleStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ClassInstanceCreation::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::OperationExpression::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::MethodInvocation::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::ReturnStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::VariableDeclarationStatement::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::AssignmentExpression::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

AST::Modifier::Modifier(const std::string &NameCode)
    : Code(NameModifierCode.at(NameCode)) {}

const AST::ModifierCode &AST::Modifier::getCode() const { return Code; }

void AST::Modifier::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::Identifier::Identifier(const std::string &Name) : Name(Name) {}

const std::string &AST::Identifier::getName() const { return Name; }

void AST::Identifier::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::PrimitiveType::PrimitiveType(const std::string &Type) : Type(Type) {}

const std::string &AST::PrimitiveType::getType() const { return Type; }

void AST::PrimitiveType::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::VoidType::VoidType(const std::string &Type) : Type(Type) {}

const std::string &AST::VoidType::getType() const { return Type; }

void AST::VoidType::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::ASSIGN::ASSIGN(const std::string &Symbol) : Symbol(Symbol) {}

const std::string &AST::ASSIGN::getSymbol() const { return Symbol; }

void AST::ASSIGN::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::BinaryOperator::BinaryOperator(const std::string &BinaryOperatorSymbol) : BinaryOperatorSymbol(BinaryOperatorSymbol) {}

const std::string &AST::BinaryOperator::getBinaryOperatorSymbol() const { return BinaryOperatorSymbol; }

void AST::BinaryOperator::accept(Visitor &Visitor) const { Visitor.visit(*this); }

AST::UnaryOperator::UnaryOperator(const std::string &UnaryOperatorSymbol) : UnaryOperatorSymbol(UnaryOperatorSymbol) {}

const std::string &AST::UnaryOperator::getUnaryOperatorSymbol() const { return UnaryOperatorSymbol; }

void AST::UnaryOperator::accept(Visitor &Visitor) const { Visitor.visit(*this); }