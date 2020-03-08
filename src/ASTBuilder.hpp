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
void decIntLiteralVisit(const Parse::Node &ParseNode, Node &ASTNode);
void booleanLiteralVisit(const Parse::Node &ParseNode, Node &ASTNode);
void characterLiteralVisit(const Parse::Node &ParseNode, Node &ASTNode);
void stringLiteralVisit(const Parse::Node &ParseNode, Node &ASTNode);
void nullLiteralVisit(const Parse::Node &ParseNode, Node &ASTNode);
void thisExpressionVisit(const Parse::Node &ParseNode, Node &ASTNode);
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

    {"Name", inodeVisit<Name>}, // Name -> Name DOT IDENTIFIER
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

    {"ASSIGN", assignVisit}, // ASSIGN -> =
    {"Expression", inodeVisit<Expression>}, // Expression -> AssignmentExpression, OperationExpression

    {"AssignmentExpression", inodeVisit<AssignmentExpression>}, // AssignmentExpression -> Name ASSIGN Expression, FieldAccess ASSIGN Expression, ArrayAccess ASSIGN Expression

    {"OperationExpression", inodeVisit<OperationExpression>}, // OperationExpression -> UnaryExpression,  OperationExpression BinaryOperator UnaryExpression, OperationExpression InstanceOperator (ArrayType | SimpleType)
    {"BinaryOperator", binaryOperatorVisit}, // BinaryOperator -> +, -, /, *, %, ==, <=, >=, <, >, !=, ||, &&, &, |, ~
    {"InstanceOperator", binaryOperatorVisit}, // InstanceOperator -> INSTANCEOF

    {"UnaryExpression", dispatchChildren}, // UnaryExpression -> SUBTRACTION UnaryExpression, UnaryExpressionNotMinus
    {"SUBTRACTION", unaryOperatorVisit}, // SUBTRACTION -> -
    {"UnaryExpressionNotMinus", dispatchChildren}, // UnaryExpressionNotMinus -> EXCLAMATION UnaryExpression, CastExpression, PostfixExpression
    {"EXCLAMATION", unaryOperatorVisit}, // EXCLAMATION -> !
    {"CastExpression", inodeVisit<CastExpression>}, // CastExpression -> (Expression) UnaryExpressionNotMinus, (PrimitiveType | ArrayType) UnaryExpression
    {"PostfixExpression", dispatchChildren}, // PostfixExpression -> Name, PrimaryExpression
    {"PrimaryExpression", dispatchChildren}, // PrimaryExpression -> PrimaryExpressionNoMultiArray, ArrayAccess, ArrayCreation

    {"PrimaryExpressionNoMultiArray", dispatchChildren}, // PrimaryExpressionNoMultiArray -> Literal, (Expression), MethodInvocation, ThisExpression, FieldAccess, ClassInstanceCreation
    {"Literal", dispatchChildren}, // Literal -> DEC_INT_LITERAL, BOOLEAN_LITERAL, CHAR_LITERAL, STRING_LITERAL, NULL_LITERAL
    {"DEC_INT_LITERAL", decIntLiteralVisit}, // DEC_INT_LITERAL -> [1-9][0-9]*
    {"BOOLEAN_LITERAL", booleanLiteralVisit}, // BOOLEAN_LITERAL -> true|false
    {"CHAR_LITERAL", characterLiteralVisit}, // CHAR_LITERAL -> '*'
    {"STRING_LITERAL", stringLiteralVisit}, // STRING_LITERAL -> "*"
    {"NULL_LITERAL", nullLiteralVisit}, // NULL_LITERAL -> null

    {"MethodInvocation", inodeVisit<MethodInvocation>}, // MethodInvocation -> PrimaryExpression DOT IDENTIFIER (ArgumentList), Name (ArgumentList)
    {"ArgumentList", inodeVisit<ArgumentList>}, // ArgumentList -> Arguments
    {"Arguments", dispatchChildren}, // Arguments -> Arguments COMMA Expression, Expression

    {"ThisExpression", thisExpressionVisit}, // ThisExpression -> THIS
    {"FieldAccess", inodeVisit<FieldAccess>}, // FieldAccess -> PrimaryExpression DOT IDENTIFIER
    {"ClassInstanceCreation", inodeVisit<ClassInstanceCreation>}, // ClassInstanceCreation -> NEW SimpleType LPAREN ArgumentList RPAREN

    {"ArrayAccess", inodeVisit<ArrayAccess>}, // ArrayAccess -> PrimaryExpressionNoMultiArray LBRKT Expression RBRKT, Name LBRKT Expression RBRKT
    {"ArrayCreation", inodeVisit<ArrayCreation>}, // ArrayCreation -> NEW (PrimitiveType|SimpleType) LBRKT Expression RBRKT

    {"Block", inodeVisit<Block>}, // Block -> LBRAC StatementList RBRAC
    {"StatementList", dispatchChildren}, // StatementList -> StatementList Statement
    {"Statement", dispatchChildren}, // Statement -> SimpleStatement, IfThenStatement, IfThenElseStatement, WhileStatement, ForStatement
    // {"SimpleStatement", inodeVisit<SimpleStatement>},
    {"SimpleStatement", dispatchChildren}, // SimpleStatement -> Block, StatementExpression, ReturnStatement, VariableDeclarationStatement

    {"StatementExpression", dispatchChildren}, // StatementExpression -> ExpressionStatement SEMI
    {"ExpressionStatement", dispatchChildren}, // ExpressionStatement -> ClassInstanceCreation, MethodInvocation, AssignmentExpression

    {"ReturnStatement", inodeVisit<ReturnStatement>}, // ReturnStatement -> RETURN Expression SEMI, RETURN SEMI
    {"VariableDeclarationStatement", dispatchChildren}, // VariableDeclarationStatement -> VariableDeclarator SEMI
    {"VariableDeclarator", inodeVisit<VariableDeclaration>}, // VariableDeclarator -> SingleVariableDeclaration ASSIGN Expression

    {"IfThenStatement", inodeVisit<IfThenStatement>}, // IfThenStatement -> IF LPAREN Expression RPAREN Statement
    {"IfThenElseStatement", inodeVisit<IfThenElseStatement>}, // IfThenElseStatement -> IF LPAREN Expression RPAREN StatementNoShortIf ELSE Statement
    {"WhileStatement", inodeVisit<WhileStatement>}, // WhileStatement -> WHILE LPAREN Expression RPAREN Statement
    {"ForStatement", inodeVisit<ForStatement>}, // ForStatement -> FOR LPAREN ForInit SEMI Expression SEMI ForUpdate RPAREN Statement
    {"ForInit", inodeVisit<ForInit>}, // ForInit -> ExpressionStatement, VariableDeclarator
    {"ForUpdate", inodeVisit<ForUpdate>}, // ForUpdate -> ExpressionStatement

    {"StatementNoShortIf", dispatchChildren}, // StatementNoShortIf -> SimpleStatement, IfThenElseStatementNoShortIf, WhileStatementNoShortIf, ForStatementNoShortIf
    {"IfThenElseStatementNoShortIf", inodeVisit<IfThenElseStatement>}, // IfThenElseStatementNoShortIf -> IF LPAREN Expression RPAREN StatementNoShortIf ELSE StatementNoShortIf
    {"WhileStatementNoShortIf", inodeVisit<WhileStatement>}, // WhileStatementNoShortIf WHILE LPAREN Expression RPAREN StatementNoShortIf
    {"ForStatementNoShortIf", inodeVisit<ForStatement>}, // ForStatementNoShortIf -> FOR LPAREN ForInit SEMI Expression SEMI ForUpdate RPAREN StatementNoShortIf

};

} // namespace AST

#endif // ASTBUILDER_HPP
