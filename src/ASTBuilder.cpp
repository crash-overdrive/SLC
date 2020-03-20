#include "ASTBuilder.hpp"

template <typename T>
void AST::inodeVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  std::unique_ptr<T> decl = std::make_unique<T>();
  dispatchChildren(parseNode, *decl);
  astNode.addChild(std::move(decl));
}

void AST::identifierVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  astNode.addChild(std::make_unique<AST::Identifier>(parseNode.getTag()));
}

void AST::modifierVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getName();
  astNode.addChild(std::make_unique<AST::Modifier>(name));
}

void AST::primitiveTypeVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getName();
  astNode.addChild(std::make_unique<AST::PrimitiveType>(name));
}

void AST::voidTypeVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::VoidType>(name));
}

void AST::assignVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::ASSIGN>(name));
}

void AST::binaryOperatorVisit(const Parse::Node &parseNode,
                              AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getTag();
  astNode.addChild(std::make_unique<AST::BinaryOperator>(name));
}

void AST::unaryOperatorVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::UnaryOperator>(name));
}

void AST::decIntLiteralVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::DecIntLiteral>(name));
}

void AST::booleanLiteralVisit(const Parse::Node &parseNode,
                              AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::BooleanLiteral>(name));
}

void AST::stringLiteralVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::StringLiteral>(name));
}

void AST::characterLiteralVisit(const Parse::Node &parseNode,
                                AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::CharacterLiteral>(name));
}

void AST::nullLiteralVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<AST::NullLiteral>(name));
}

void AST::thisExpressionVisit(const Parse::Node &parseNode,
                              AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getTag();
  astNode.addChild(std::make_unique<AST::ThisExpression>(name));
}

void AST::dispatchChildren(const Parse::Node &parseNode, Node &astNode) {
  for (const auto &child : parseNode.getChildren()) {
    AST::dispatch(*child, astNode);
  }
}

void AST::dispatch(const Parse::Node &parseNode, Node &astNode) {
  auto it = AST::parseVisit.find(parseNode.getName());
  if (it != AST::parseVisit.end()) {
    (it->second)(parseNode, astNode);
  }
}
