#include "ASTBuilder.hpp"

void AST::packageDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode) {
  std::unique_ptr<PackageDeclaration> Decl = std::make_unique<PackageDeclaration>();
  dispatchChildren(ParseNode, *Decl);
  ASTNode.addChild(std::move(Decl));
}

void AST::classDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode) {
  std::unique_ptr<ClassDeclaration> Decl = std::make_unique<ClassDeclaration>();
  dispatchChildren(ParseNode, *Decl);
  ASTNode.addChild(std::move(Decl));
}

void AST::interfaceDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode) {
  std::unique_ptr<InterfaceDeclaration> Decl = std::make_unique<InterfaceDeclaration>();
  dispatchChildren(ParseNode, *Decl);
  ASTNode.addChild(std::move(Decl));
}

void AST::identifierVisit(const Parse::Node &ParseNode, Node &ASTNode) {
  ASTNode.addChild(std::make_unique<Identifier>(ParseNode.getTag()));
}

void AST::modifierVisit(const Parse::Node &ParseNode, Node &ASTNode) {
  const std::string &Name = ParseNode.getFirstChild()->getName();
  ASTNode.addChild(std::make_unique<Modifier>(Name));
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
