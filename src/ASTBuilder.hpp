#ifndef ASTBUILDER_HPP
#define ASTBUILDER_HPP

#include "ASTNode.hpp"
#include "ParseTree.hpp"

namespace AST {

template <typename T>
void inodeVisit(const Parse::Node &ParseNode, Node &ASTNode);
void identifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void modifierVisit(const Parse::Node &ParseNode, Node &ASTNode);
void primitiveTypeVisit(const Parse::Node &ParseNode, Node &ASTNode);
void voidTypeVisit(const Parse::Node &ParseNode, Node &ASTNode);
void dispatchChildren(const Parse::Node &ParseNode, Node &ASTNode);
void dispatch(const Parse::Node &ParseNode, Node &ASTNode);

typedef void (*ParseVisitor)(const Parse::Node &ParseNode, Node &ASTNode);
const std::unordered_map<std::string, ParseVisitor> ParseVisit{
    {"Start", dispatchChildren}, // Start -> CompilationUnit

    {"CompilationUnit", dispatchChildren}, // CompilationUnit -> ImportDeclarations, PackageDeclaration, TypeDeclaration

    {"ImportDeclarations", dispatchChildren}, // ImportDeclarations -> ImportDeclarations ImportDeclaration
    {"PackageDeclaration", inodeVisit<PackageDeclaration>}, // PackageDeclaration -> PACKAGE Name SEMI
    {"TypeDeclaration", dispatchChildren}, // TypeDeclaration -> ClassDeclaration, InterfaceDeclaration

    {"ImportDeclaration", dispatchChildren}, // ImportDeclaration -> SingleTypeImportDeclaration, TypeImportOnDemandDeclaration
    {"SingleTypeImportDeclaration", inodeVisit<SingleImportDeclaration>}, // SingleTypeImportDeclaration IMPORT Name SEMI
    {"TypeImportOnDemandDeclaration", inodeVisit<DemandImportDeclaration>}, // TypeImportOnDemandDeclaration IMPORT Name DOT MULTIPLICATION SEMI

    {"Name", inodeVisit<Name>}, // Name -> Name DOT IDENTIFIER // TODO: fix this from having multiple names
    {"IDENTIFIER", identifierVisit},

    {"InterfaceDeclaration", inodeVisit<InterfaceDeclaration>}, // InterfaceDeclaration -> ModifierList INTERFACE IDENTIFIER Extensions LBRAC InterfaceBodyDeclarationList RBRAC

    {"ModifierList", dispatchChildren}, // ModifierList -> ModifierList Modifier // TODO: group these?
    {"Modifier", modifierVisit}, // Modifier -> PUBLIC, PRIVATE, PROTECTED, STATIC, ABSTRACT, FINAL, NATIVE

    {"Extensions", inodeVisit<Extensions>}, // Extensions -> EXTENDS ExtensionList
    {"ExtensionList", dispatchChildren}, // ExtensionList -> ExtensionList COMMA Name

    {"InterfaceBodyDeclarationList", dispatchChildren}, // InterfaceBodyDeclarationList -> InterfaceBodyDeclarationList InterfaceBodyDeclaration
    {"InterfaceBodyDeclaration", dispatchChildren}, // InterfaceBodyDeclaration -> InterfaceMethodDeclaration
    {"InterfaceMethodDeclaration", inodeVisit<MethodDeclaration>}, // InterfaceMethodDeclaration -> MethodHeader SEMI

    {"MethodHeader", dispatchChildren}, // MethodHeader -> ModifierList (Type|VOID) IDENTIFIER LPAREN FormalParameterList RPAREN

    {"Type", dispatchChildren}, // Type -> PrimitiveType, ArrayType, SimpleType
    {"PrimitiveType", primitiveTypeVisit}, // PrimitiveType -> BYTE, SHORT, CHAR, INT, BOOLEAN
    {"ArrayType", inodeVisit<ArrayType>}, // ArrayType -> PrimitiveType[], Name[]
    {"SimpleType", inodeVisit<SimpleType>}, // SimpleType -> Name
    {"VOID", voidTypeVisit}, // VOID

    // {"FormalParameterList", inodeVisit<FormalParameterList>},
    {"FormalParameterList", dispatchChildren}, // FormalParameterList -> FormalParameterList COMMA SingleVariableDeclaration
    {"SingleVariableDeclaration", inodeVisit<SingleVariableDeclaration>}, // SingleVariableDeclaration -> Type IDENTIFIER

    {"ClassDeclaration", inodeVisit<ClassDeclaration>}, // ClassDeclaration -> ModifierList CLASS IDENTIFIER Super Interfaces LBRAC ClassBodyDeclarationList RBRAC

    {"Super", inodeVisit<Super>}, // Super -> EXTENDS Name

    {"Interfaces", inodeVisit<Interfaces>}, // Interfaces -> IMPLEMENTS InterfaceList
    {"InterfaceList", dispatchChildren}, // InterfaceList -> InterfaceList COMMA Name, Name

    {"ClassBodyDeclarationList", dispatchChildren}, // ClassBodyDeclarationList -> ClassBodyDeclarationList ClassBodyDeclaration
    {"ClassBodyDeclaration", dispatchChildren}, // ClassBodyDeclaration -> FieldDeclaration, ConstructorDeclaration, ClassMethodDeclaration

    {"FieldDeclaration", inodeVisit<FieldDeclaration>}, // FieldDeclaration -> ModifierList Type IDENTIFIER ASSIGN Expression SEMI

    {"ConstructorDeclaration", inodeVisit<ConstructorDeclaration>}, // ConstructorDeclaration -> ModifierList IDENTIFIER LPAREN FormalParameterList RPAREN Block

    {"ClassMethodDeclaration", inodeVisit<MethodDeclaration>}, // ClassMethodDeclaration -> MethodHeader Block, MethodHeader SEMI

    // ASSIGN
    // Expression
    // Block




};

} // namespace AST

#endif // ASTBUILDER_HPP
