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
void assignVisit(const Parse::Node &ParseNode, Node &ASTNode);
void binaryOperatorVisit(const Parse::Node &ParseNode, Node &ASTNode);
void unaryOperatorVisit(const Parse::Node &ParseNode, Node &ASTNode);
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
    // {"Name", dispatchChildren},
    {"IDENTIFIER", identifierVisit},

    {"InterfaceDeclaration", inodeVisit<InterfaceDeclaration>}, // InterfaceDeclaration -> ModifierList INTERFACE IDENTIFIER Extensions LBRAC InterfaceBodyDeclarationList RBRAC

    {"ModifierList", dispatchChildren}, // ModifierList -> ModifierList Modifier
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

    {"ASSIGN", assignVisit},
    {"Expression", dispatchChildren}, // Expression -> AssignmentExpression, OperationExpression

    {"AssignmentExpression", inodeVisit<AssignmentExpression>}, // AssignmentExpression -> Name ASSIGN Expression, FieldAccess ASSIGN Expression, ArrayAccess ASSIGN Expression

    {"OperationExpression", inodeVisit<OperationExpression>}, // OperationExpression -> UnaryExpression,  OperationExpression BinaryOperator UnaryExpression, OperationExpression InstanceOperator (ArrayType | SimpleType)
    {"BinaryOperator", binaryOperatorVisit}, // BinaryOperator -> +, -, /, *, %, ==, <=, >=, <, >, !=, ||, &&, &, |, ~
    {"InstanceOperator", binaryOperatorVisit}, // InstanceOperator -> INSTANCEOF

    {"UnaryExpression", dispatchChildren}, // UnaryExpression -> SUBTRACTION UnaryExpression, UnaryExpressionNotMinus
    {"SUBTRACTION", unaryOperatorVisit}, // SUBTRACTION -> -
    {"UnaryExpressionNotMinus", dispatchChildren}, // UnaryExpressionNotMinus -> EXCLAMATION UnaryExpression, CastExpression, PostfixExpression
    {"EXCLAMATION", unaryOperatorVisit}, // EXCLAMATION -> !
    // {"CastExpression", ...}, // TODO
    // {"PostFixExpression", ...}, // TODO

    {"Block", inodeVisit<Block>}, // Block -> LBRAC StatementList RBRAC
    {"StatementList", dispatchChildren}, // StatementList -> StatementList Statement
    {"Statement", dispatchChildren}, // Statement -> SimpleStatement, IfThenStatement, IfThenElseStatement, WhileStatement, ForStatement
    // TODO: check if we need this or should we just dispatch children here?
    // {"SimpleStatement", inodeVisit<SimpleStatement>},
    {"SimpleStatement", dispatchChildren}, // SimpleStatement -> Block, StatementExpression, ReturnStatement, VariableDeclarationStatement

    {"StatementExpression", dispatchChildren}, // StatementExpression -> ExpressionStatement SEMI
    {"ExpressionStatement", dispatchChildren}, // ExpressionStatement -> ClassInstanceCreation, MethodInvocation, AssignmentExpression

    {"ClassInstanceCreation", inodeVisit<ClassInstanceCreation>}, // ClassInstanceCreation -> NEW SimpleType LPAREN ArgumentList RPAREN
    {"ArgumentList", dispatchChildren}, // ArgumentList -> ArgumentList COMMA Expression

    {"MethodInvocation", inodeVisit<MethodInvocation>},

    {"ReturnStatement", inodeVisit<ReturnStatement>},
    {"VariableDeclarationStatement", inodeVisit<VariableDeclarationStatement>}, // VariableDeclarationStatement -> VariableDeclarator SEMI
    {"VariableDeclarator", dispatchChildren}, // VariableDeclarator -> SingleVariableDeclaration ASSIGN Expression

    {"IfThenStatement", inodeVisit<IfThenStatement>},
    {"IfThenElseStatement", inodeVisit<IfThenElseStatement>},
    {"WhileStatement", inodeVisit<WhileStatement>},
    {"ForStatement", inodeVisit<ForStatement>},

};

} // namespace AST

#endif // ASTBUILDER_HPP
