#include "ASTBuilder.hpp"

void AST::classDeclarationVisit(Node &ASTNode, const Parse::Node &ParseNode) {
  std::unique_ptr<TypeDeclaration> Decl = std::make_unique<TypeDeclaration>();
  dispatchChildren(*Decl, ParseNode);
  ASTNode.addChild(std::move(Decl));
}

void AST::interfaceDeclarationVisit(Node &ASTNode,
                                    const Parse::Node &ParseNode) {
  std::unique_ptr<TypeDeclaration> Decl = std::make_unique<TypeDeclaration>(false);
  dispatchChildren(*Decl, ParseNode);
  ASTNode.addChild(std::move(Decl));
}

void AST::identifierVisit(Node &ASTNode, const Parse::Node &ParseNode) {
  ASTNode.addChild(std::make_unique<Identifier>(ParseNode.getTag()));
}

void AST::modifierVisit(Node &ASTNode, const Parse::Node &ParseNode) {
  const std::string &Name = ParseNode.getFirstChild()->getName();
  ASTNode.addChild(std::make_unique<Modifier>(Name));
}

void AST::dispatchChildren(Node &ASTNode, const Parse::Node &ParseNode) {
  for (const auto &Child : ParseNode.getChildren()) {
    AST::dispatch(ASTNode, *Child);
  }
}

void AST::dispatch(Node &ASTNode, const Parse::Node &ParseNode) {
  auto it = AST::ParseVisit.find(ParseNode.getName());
  if (it != AST::ParseVisit.end()) {
    (it->second)(ASTNode, ParseNode);
  }
}
