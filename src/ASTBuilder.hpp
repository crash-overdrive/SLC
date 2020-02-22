#ifndef ASTBUILDER_HPP
#define ASTBUILDER_HPP

#include "ASTNode.hpp"
#include "ParseTree.hpp"

namespace AST {

void classDeclarationVisit(Node &ASTNode, const Parse::Node &ParseNode);
void interfaceDeclarationVisit(Node &ASTNode, const Parse::Node &ParseNode);
void identifierVisit(Node &ASTNode, const Parse::Node &ParseNode);
void modifierVisit(Node &ASTNode, const Parse::Node &ParseNode);
void dispatchChildren(Node &ASTNode, const Parse::Node &ParseNode);
void dispatch(Node &ASTNode, const Parse::Node &ParseNode);

typedef void (*ParseVisitor)(Node &ASTNode, const Parse::Node &ParseNode);
const std::unordered_map<std::string, ParseVisitor> ParseVisit {
    {"Start", dispatchChildren},
    {"TypeDeclaration", dispatchChildren},
    {"ClassDeclaration", classDeclarationVisit},
    {"InterfaceDeclaration", interfaceDeclarationVisit},
    {"InterfaceBodyDeclaration", dispatchChildren},
    {"ClassBodyDeclaration", dispatchChildren},
    {"ModifierList", dispatchChildren},
    {"Modifier", modifierVisit},
    {"IDENTIFIER", identifierVisit},
};

} // namespace AST

#endif // ASTBUILDER_HPP
