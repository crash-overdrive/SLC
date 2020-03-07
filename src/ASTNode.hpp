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

const std::unordered_map<std::string, ModifierCode> NameModifierCode{
    {"PUBLIC", ModifierCode::Public},       {"PRIVATE", ModifierCode::Private},
    {"PROTECTED", ModifierCode::Protected}, {"STATIC", ModifierCode::Static},
    {"ABSTRACT", ModifierCode::Abstract},   {"FINAL", ModifierCode::Final},
    {"NATIVE", ModifierCode::Native},
};

const std::unordered_map<ModifierCode, std::string> ModifierCodeName{
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
  void addChild(std::unique_ptr<Node> Child);
  const std::vector<std::unique_ptr<Node>> &getChildren() const;
  virtual void accept(Visitor &Visitor) const = 0;

private:
  std::vector<std::unique_ptr<Node>> Children;
};

class Start : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class SingleImportDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class DemandImportDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class PackageDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ClassDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class InterfaceDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class MethodDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class FieldDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ConstructorDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Extensions : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Name : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ArrayType : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class SimpleType : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class FormalParameterList : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class SingleVariableDeclaration : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Super : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Interfaces : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Block : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class IfThenStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class IfThenElseStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class WhileStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ForStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class SimpleStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ClassInstanceCreation : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class OperationExpression : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class MethodInvocation : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class ReturnStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class VariableDeclarationStatement : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class AssignmentExpression : public Node {
public:
  void accept(Visitor &Visitor) const override;
};

class Modifier : public Node {
public:
  Modifier(const std::string &NameCode);
  const ModifierCode &getCode() const;
  void accept(Visitor &Visitor) const override;

private:
  ModifierCode Code;
};

class Identifier : public Node {
public:
  Identifier(const std::string &Name);
  const std::string &getName() const;
  void accept(Visitor &Visitor) const override;

private:
  std::string Name;
};

class PrimitiveType : public Node {
public:
  PrimitiveType(const std::string &Type);
  const std::string &getType() const;
  void accept(Visitor &Visitor) const override;

private:
  std::string Type;
};

class VoidType : public Node {
public:
  VoidType(const std::string &Type);
  const std::string &getType() const;
  void accept(Visitor &Visitor) const override;

private:
  std::string Type;
};

class ASSIGN : public Node {
public:
  ASSIGN(const std::string &Symbol);
  const std::string &getSymbol() const;
  void accept(Visitor &Visitor) const override;

private:
  std::string Symbol;
};

class Operator : public Node {
public:
  Operator(const std::string &Operator);
  const std::string &getOperatorSymbol() const;
  void accept(Visitor &Visitor) const override;

private:
  std::string OperatorSymbol;
};

} // namespace AST

#endif // ASTNODE_HPP
