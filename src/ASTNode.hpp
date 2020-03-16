#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <memory>
#include <unordered_map>
#include <vector>

namespace AST {

class Visitor;

enum class ModifierCode {
  Public,
  Private,
  Protected,
  Static,
  Abstract,
  Final,
  Native,
};

const std::unordered_map<std::string, ModifierCode> nameModifierCode{
    {"PUBLIC", ModifierCode::Public},       {"PRIVATE", ModifierCode::Private},
    {"PROTECTED", ModifierCode::Protected}, {"STATIC", ModifierCode::Static},
    {"ABSTRACT", ModifierCode::Abstract},   {"FINAL", ModifierCode::Final},
    {"NATIVE", ModifierCode::Native},
};

const std::unordered_map<ModifierCode, std::string> modifierCodeName{
    {ModifierCode::Public, "PUBLIC"},
    {ModifierCode::Private, "PRIVATE"},
    {ModifierCode::Protected, "PROTECTED"},
    {ModifierCode::Protected, "PROTECTED"},
    {ModifierCode::Static, "STATIC"},
    {ModifierCode::Abstract, "ABSTRACT"},
    {ModifierCode::Final, "FINAL"},
    {ModifierCode::Native, "NATIVE"},
};

class Node {
public:
  virtual ~Node() = default;
  void addChild(std::unique_ptr<Node> child);
  const std::vector<std::unique_ptr<Node>> &getChildren() const;
  virtual void accept(Visitor &visitor) const = 0;

private:
  std::vector<std::unique_ptr<Node>> children;
};

class Start : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class SingleImportDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class DemandImportDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class PackageDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ClassDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class InterfaceDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class MethodDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class FieldDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ConstructorDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Extensions : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Name : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ArrayType : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class SimpleType : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Expression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class CastExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class SingleVariableDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Super : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Interfaces : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Block : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class IfThenStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class IfThenElseStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class WhileStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ForStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class SimpleStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ClassInstanceCreation : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class OperationExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class MethodInvocation : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ReturnStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class VariableDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class AssignmentExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class UnaryExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ArgumentList : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class FieldAccess : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ArrayAccess : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ArrayCreation : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ForInit : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ForUpdate : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Modifier : public Node {
public:
  Modifier(const std::string &nameCode);
  const ModifierCode &getCode() const;
  void accept(Visitor &visitor) const override;

private:
  ModifierCode code;
};

class Identifier : public Node {
public:
  Identifier(const std::string &name);
  const std::string &getName() const;
  void accept(Visitor &visitor) const override;

private:
  std::string name;
};

class PrimitiveType : public Node {
public:
  PrimitiveType(const std::string &type);
  const std::string &getType() const;
  void accept(Visitor &visitor) const override;

private:
  std::string type;
};

class VoidType : public Node {
public:
  VoidType(const std::string &type);
  const std::string &getType() const;
  void accept(Visitor &visitor) const override;

private:
  std::string type;
};

class ASSIGN : public Node {
public:
  ASSIGN(const std::string &symbol);
  const std::string &getSymbol() const;
  void accept(Visitor &visitor) const override;

private:
  std::string symbol;
};

class BinaryOperator : public Node {
public:
  BinaryOperator(const std::string &binaryOperator);
  const std::string &getBinaryOperatorSymbol() const;
  void accept(Visitor &visitor) const override;

private:
  std::string binaryOperatorSymbol;
};

class UnaryOperator : public Node {
public:
  UnaryOperator(const std::string &unaryOperator);
  const std::string &getUnaryOperatorSymbol() const;
  void accept(Visitor &visitor) const override;

private:
  std::string unaryOperatorSymbol;
};

class DecIntLiteral : public Node {
public:
  DecIntLiteral(const std::string &literal);
  const std::string &getLiteral() const;
  void accept(Visitor &visitor) const override;

private:
  std::string literal;
};

class BooleanLiteral : public Node {
public:
  BooleanLiteral(const std::string &literal);
  const std::string &getLiteral() const;
  void accept(Visitor &visitor) const override;

private:
  std::string literal;
};

class CharacterLiteral : public Node {
public:
  CharacterLiteral(const std::string &literal);
  const std::string &getLiteral() const;
  void accept(Visitor &visitor) const override;

private:
  std::string literal;
};

class StringLiteral : public Node {
public:
  StringLiteral(const std::string &literal);
  const std::string &getLiteral() const;
  void accept(Visitor &visitor) const override;

private:
  std::string literal;
};

class NullLiteral : public Node {
public:
  NullLiteral(const std::string &literal);
  const std::string &getLiteral() const;
  void accept(Visitor &visitor) const override;

private:
  std::string literal;
};


class ThisExpression : public Node {
public:
  ThisExpression(const std::string &expression);
  const std::string &getExpression() const;
  void accept(Visitor &visitor) const override;

private:
  std::string expression;
};

std::ostream &operator<<(std::ostream &stream, const Node &node);

} // namespace AST

#endif // ASTNODE_HPP
