#ifndef ASTBUILDER_HPP
#define ASTBUILDER_HPP

#include "ASTNode.hpp"
#include "ParseTree.hpp"

namespace AST {

void packageDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode);
void classDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode);
void interfaceDeclarationVisit(const Parse::Node &ParseNode, Node &ASTNode);
void identifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void modifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void dispatchChildren(const Parse::Node &ParseNode, Node &ASTNode);
void dispatch(const Parse::Node &ParseNode, Node &ASTNode);

typedef void (*ParseVisitor)(const Parse::Node &ParseNode, Node &ASTNode);
const std::unordered_map<std::string, ParseVisitor> ParseVisit {
    {"Start", dispatchChildren},
    {"CompilationUnit", dispatchChildren},
    {"PackageDeclaration", packageDeclarationVisit},
    {"Name", dispatchChildren},
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
