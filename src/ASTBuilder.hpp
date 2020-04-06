#ifndef ASTBUILDER_HPP
#define ASTBUILDER_HPP

#include "ASTNode.hpp"
#include "ParseTree.hpp"

namespace AST {

template <typename T>
void inodeVisit(const Parse::Node &parseNode, Node &astNode);
template <typename T>
void expressionVisit(const Parse::Node &parseNode, AST::Node &astNode);
template <typename T>
void leafVisit(const Parse::Node &parseNode, AST::Node &astNode);

void identifierVisit(const Parse::Node &parseNode, Node &astNode);
void modifierVisit(const Parse::Node &parseNode, Node &astNode);
void primitiveTypeVisit(const Parse::Node &parseNode, Node &astNode);
void thisExpressionVisit(const Parse::Node &parseNode, Node &astNode);
void dispatchChildren(const Parse::Node &parseNode, Node &astNode);
void dispatch(const Parse::Node &parseNode, Node &astNode);

using ParseVisitor = void (*)(const Parse::Node &, Node &);
const std::unordered_map<std::string, ParseVisitor> parseVisit{
    {"Start", dispatchChildren},
    {"CompilationUnit", dispatchChildren},
    {"ImportDeclarations", dispatchChildren},
    {"PackageDeclaration", inodeVisit<PackageDeclaration>},
    {"TypeDeclaration", dispatchChildren},
    {"ImportDeclaration", dispatchChildren},
    {"SingleTypeImportDeclaration", inodeVisit<SingleImportDeclaration>},
    {"TypeImportOnDemandDeclaration", inodeVisit<DemandImportDeclaration>},
    {"Name", inodeVisit<Name>},
    {"IDENTIFIER", leafVisit<Identifier>},
    {"InterfaceDeclaration", inodeVisit<InterfaceDeclaration>},
    {"ModifierList", dispatchChildren},
    {"Modifier", modifierVisit},
    {"Extensions", dispatchChildren},
    {"ExtensionList", dispatchChildren},
    {"ExtensionType", inodeVisit<Extensions>},
    {"InterfaceBodyDeclarationList", dispatchChildren},
    {"InterfaceBodyDeclaration", dispatchChildren},
    {"InterfaceMethodDeclaration", inodeVisit<MethodDeclaration>},
    {"MethodHeader", dispatchChildren},
    {"Type", dispatchChildren},
    {"PrimitiveType", primitiveTypeVisit},
    {"VoidType", primitiveTypeVisit},
    {"ArrayType", inodeVisit<ArrayType>},
    {"SimpleType", inodeVisit<SimpleType>},
    {"FormalParameterList", dispatchChildren},
    {"SingleVariableDeclaration", inodeVisit<SingleVariableDeclaration>},
    {"ClassDeclaration", inodeVisit<ClassDeclaration>},
    {"Super", inodeVisit<Super>},
    {"Interfaces", dispatchChildren},
    {"InterfaceList", dispatchChildren},
    {"InterfaceType", inodeVisit<Interfaces>},
    {"ClassBodyDeclarationList", dispatchChildren},
    {"ClassBodyDeclaration", dispatchChildren},
    {"FieldDeclaration", inodeVisit<FieldDeclaration>},
    {"ConstructorDeclaration", inodeVisit<ConstructorDeclaration>},
    {"ClassMethodDeclaration", inodeVisit<MethodDeclaration>},

    {"StatementList", dispatchChildren},
    {"Statement", dispatchChildren},
    {"SimpleStatement", dispatchChildren},
    {"Expression", dispatchChildren},
    {"StatementExpression", inodeVisit<ExpressionStatement>},
    {"ExpressionStatement", dispatchChildren},
    {"AssignmentExpression", dispatchChildren},
    {"ReturnStatement", inodeVisit<ReturnStatement>},
    {"VariableDeclarationStatement", dispatchChildren},
    {"VariableDeclarator", inodeVisit<VariableDeclaration>},
    {"IfThenStatement", inodeVisit<IfThenStatement>},
    {"IfThenElseStatement", inodeVisit<IfThenElseStatement>},
    {"WhileStatement", inodeVisit<WhileStatement>},
    {"ForStatement", inodeVisit<ForStatement>},
    {"ForInit", inodeVisit<ForInit>},
    {"ForUpdate", inodeVisit<ForUpdate>},
    {"StatementNoShortIf", dispatchChildren},
    {"IfThenElseStatementNoShortIf", inodeVisit<IfThenElseStatement>},
    {"WhileStatementNoShortIf", inodeVisit<WhileStatement>},
    {"ForStatementNoShortIf", inodeVisit<ForStatement>},

    {"Assignment", inodeVisit<AssignmentExpression>},
    {"ConditionalOrExpression", expressionVisit<BinaryExpression>},
    {"ConditionalAndExpression", expressionVisit<BinaryExpression>},
    {"InclusiveOrExpression", expressionVisit<BinaryExpression>},
    {"ExclusiveOrExpression", expressionVisit<BinaryExpression>},
    {"AndExpression", expressionVisit<BinaryExpression>},
    {"EqualityExpression", expressionVisit<BinaryExpression>},
    {"RelationalExpression", expressionVisit<BinaryExpression>},
    {"AdditiveExpression", expressionVisit<BinaryExpression>},
    {"MultiplicativeExpression", expressionVisit<BinaryExpression>},
    {"UnaryExpression", expressionVisit<UnaryExpression>},
    {"UnaryExpressionNotMinus", expressionVisit<UnaryExpression>},
    {"InstanceOfExpression", expressionVisit<InstanceOfExpression>},
    {"CastExpression", inodeVisit<CastExpression>},
    {"PostfixExpression", dispatchChildren},
    {"Primary", dispatchChildren},
    {"PrimaryExpressionNoNewArray", dispatchChildren},
    {"MethodNameInvocation", inodeVisit<MethodNameInvocation>},
    {"MethodPrimaryInvocation", inodeVisit<MethodPrimaryInvocation>},
    {"ThisExpression", thisExpressionVisit},
    {"FieldAccess", inodeVisit<FieldAccess>},
    {"ClassInstanceCreation", inodeVisit<ClassInstanceCreation>},
    {"ArrayAccess", inodeVisit<ArrayAccess>},
    {"ArrayCreation", inodeVisit<ArrayCreation>},
    {"Literal", dispatchChildren},

    {"ADDITION", leafVisit<Operator>},
    {"SUBSTRACTION", leafVisit<Operator>},
    {"MULTIPLICATION", leafVisit<Operator>},
    {"DIVISION", leafVisit<Operator>},
    {"MODULUS", leafVisit<Operator>},
    {"EQUAL", leafVisit<Operator>},
    {"LESSER_OR_EQUAL", leafVisit<Operator>},
    {"GREATER_OR_EQUAL", leafVisit<Operator>},
    {"LESSER", leafVisit<Operator>},
    {"GREATER", leafVisit<Operator>},
    {"NOT_EQUAL", leafVisit<Operator>},
    {"OR", leafVisit<Operator>},
    {"AND", leafVisit<Operator>},
    {"BITWISEAND", leafVisit<Operator>},
    {"BITWISEOR", leafVisit<Operator>},
    {"BITWISEXOR", leafVisit<Operator>},
    {"EXCLAMATION", leafVisit<Operator>},

    {"DEC_INT_LITERAL", leafVisit<DecIntLiteral>},
    {"BOOLEAN_LITERAL", leafVisit<BooleanLiteral>},
    {"CHAR_LITERAL", leafVisit<CharacterLiteral>},
    {"STRING_LITERAL", leafVisit<StringLiteral>},
    {"NULL_LITERAL", leafVisit<NullLiteral>},

    {"ArgumentList", inodeVisit<ArgumentList>},
    {"Arguments", dispatchChildren},
    {"Block", inodeVisit<Block>},
};

} // namespace AST

#endif // ASTBUILDER_HPP
