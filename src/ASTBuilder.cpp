#include "ASTBuilder.hpp"

template <typename T>
void AST::inodeVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  std::unique_ptr<T> decl = std::make_unique<T>();
  dispatchChildren(parseNode, *decl);
  astNode.addChild(std::move(decl));
}

template <typename T>
void AST::expressionVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  if (parseNode.getChildren().size() > 1) {
    std::unique_ptr<T> decl = std::make_unique<T>();
    dispatchChildren(parseNode, *decl);
    astNode.addChild(std::move(decl));
  } else {
    dispatchChildren(parseNode, astNode);
  }
}

template <typename T>
void AST::leafVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getTag();
  astNode.addChild(std::make_unique<T>(name));
}

void AST::modifierVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getName();
  astNode.addChild(std::make_unique<AST::Modifier>(std::move(name)));
}

void AST::primitiveTypeVisit(const Parse::Node &parseNode, AST::Node &astNode) {
  const std::string &name = parseNode.getFirstChild()->getName();
  astNode.addChild(std::make_unique<AST::PrimitiveType>(name));
}

void AST::thisExpressionVisit(const Parse::Node &, AST::Node &astNode) {
  astNode.addChild(std::make_unique<AST::ThisExpression>());
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
