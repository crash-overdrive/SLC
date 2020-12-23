#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace AST {

class Visitor;

class Node {
public:
  virtual ~Node() = default;
  void addChild(std::unique_ptr<Node> child);
  const std::vector<std::unique_ptr<Node>> &getChildren() const;
  const Node &getChild(size_t index) const;
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

class CastExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class CastType : public Node {
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

class ForInit : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ForCond : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ForUpdate : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ExpressionStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class VariableDeclaration : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ReturnStatement : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ClassInstanceCreation : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class AssignmentExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class BinaryExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class InstanceOfExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Operator : public Node {
public:
  Operator(const std::string &operatorSymbol);
  const std::string &getOperatorSymbol() const;
  void accept(Visitor &visitor) const override;

private:
  std::string operatorSymbol;
};

class MethodNameInvocation : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class MethodPrimaryInvocation : public Node {
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

class UnaryExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class ThisExpression : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Argument : public Node {
public:
  void accept(Visitor &visitor) const override;
};

class Modifier : public Node {
public:
  Modifier(std::string name);
  const std::string &getName() const;
  void accept(Visitor &visitor) const override;

private:
  std::string name;
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

std::ostream &operator<<(std::ostream &stream, const Node &node);

} // namespace AST

#endif // ASTNODE_HPP
