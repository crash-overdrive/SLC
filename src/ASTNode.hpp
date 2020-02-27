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

} // namespace AST

#endif // ASTNODE_HPP
