#ifndef ASTBUILDER_HPP
#define ASTBUILDER_HPP

#include "ASTNode.hpp"
#include "ParseTree.hpp"

namespace AST {

template <typename T>
void inodeVisit(const Parse::Node &ParseNode, Node &ASTNode);
void identifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void modifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void dispatchChildren(const Parse::Node &ParseNode, Node &ASTNode);
void dispatch(const Parse::Node &ParseNode, Node &ASTNode);

typedef void (*ParseVisitor)(const Parse::Node &ParseNode, Node &ASTNode);
const std::unordered_map<std::string, ParseVisitor> ParseVisit{
    {"Start", dispatchChildren},
    {"CompilationUnit", dispatchChildren},
    {"PackageDeclaration", inodeVisit<PackageDeclaration>},
    {"ImportDeclarations", dispatchChildren},
    {"ImportDeclaration", dispatchChildren},
    {"SingleTypeImportDeclaration", inodeVisit<SingleImportDeclaration>},
    {"TypeImportOnDemandDeclaration", inodeVisit<DemandImportDeclaration>},
    {"Name", dispatchChildren},
    {"TypeDeclaration", dispatchChildren},
    {"ClassDeclaration", inodeVisit<ClassDeclaration>},
    {"InterfaceDeclaration", inodeVisit<InterfaceDeclaration>},
    {"InterfaceBodyDeclaration", dispatchChildren},
    {"ClassBodyDeclaration", dispatchChildren},
    {"ModifierList", dispatchChildren},
    {"Modifier", modifierVisit},
    {"IDENTIFIER", identifierVisit},
};

} // namespace AST

#endif // ASTBUILDER_HPP
