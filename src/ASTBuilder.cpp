#include "ASTBuilder.hpp"

template <typename T>
void AST::inodeVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  std::unique_ptr<T> Decl = std::make_unique<T>();
  dispatchChildren(ParseNode, *Decl);
  ASTNode.addChild(std::move(Decl));
}

void AST::identifierVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  ASTNode.addChild(std::make_unique<AST::Identifier>(ParseNode.getTag()));
}

void AST::modifierVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getFirstChild()->getName();
  ASTNode.addChild(std::make_unique<AST::Modifier>(Name));
}

void AST::primitiveTypeVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getFirstChild()->getName();
  ASTNode.addChild(std::make_unique<AST::PrimitiveType>(Name));
}

void AST::voidTypeVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::VoidType>(Name));
}

void AST::assignVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::ASSIGN>(Name));
}

void AST::binaryOperatorVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getFirstChild()->getTag();
  ASTNode.addChild(std::make_unique<AST::BinaryOperator>(Name));
}

void AST::unaryOperatorVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::UnaryOperator>(Name));
}

void AST::decIntLiteralVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::DecIntLiteral>(Name));
}

void AST::booleanLiteralVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::BooleanLiteral>(Name));
}

void AST::stringLiteralVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::StringLiteral>(Name));
}

void AST::characterLiteralVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::CharacterLiteral>(Name));
}

void AST::nullLiteralVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getTag();
  ASTNode.addChild(std::make_unique<AST::NullLiteral>(Name));
}

void AST::thisExpressionVisit(const Parse::Node &ParseNode, AST::Node &ASTNode) {
  const std::string &Name = ParseNode.getFirstChild()->getTag();
  ASTNode.addChild(std::make_unique<AST::ThisExpression>(Name));
}

void AST::dispatchChildren(const Parse::Node &ParseNode, Node &ASTNode) {
  for (const auto &Child : ParseNode.getChildren()) {
    AST::dispatch(*Child, ASTNode);
  }
}

void AST::dispatch(const Parse::Node &ParseNode, Node &ASTNode) {
  auto it = AST::ParseVisit.find(ParseNode.getName());
  if (it != AST::ParseVisit.end()) {
    (it->second)(ParseNode, ASTNode);
  }
}
