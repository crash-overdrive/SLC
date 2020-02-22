#include <ASTNode.hpp>
#include <ASTVisitor.hpp>

void AST::Node::addChild(std::unique_ptr<Node> Child) {
  Children.emplace_back(std::move(Child));
}

void AST::Node::accept(Visitor &Visitor) const{ (void)Visitor; }

const std::vector<std::unique_ptr<AST::Node>> &AST::Node::getChildren() const {
  return Children;
}

void AST::Start::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

AST::TypeDeclaration::TypeDeclaration() : Interface(false) {}

AST::TypeDeclaration::TypeDeclaration(bool Class) : Interface(!Class) {}

bool AST::TypeDeclaration::isInterface() const { return Interface; }

void AST::TypeDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::MethodDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

void AST::FieldDeclaration::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

AST::Modifier::Modifier(const std::string &NameCode)
    : Code(NameModifierCode.at(NameCode)) {}

const AST::ModifierCode &AST::Modifier::getCode() const { return Code; }

void AST::Modifier::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}

AST::Identifier::Identifier(const std::string &Name) : Name(Name) {}

const std::string &AST::Identifier::getName() const {
  return Name;
}

void AST::Identifier::accept(Visitor &Visitor) const {
  Visitor.visit(*this);
}
