#ifndef ENVJOOSTYPE_HPP
#define ENVJOOSTYPE_HPP

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"
#include "EnvJoosBody.hpp"
#include <set>
#include <unordered_set>

namespace Env {

enum class Type {
  Class,
  Interface,
};

const std::unordered_map<Type, std::string> typeName{
    {Type::Class, "Class"},
    {Type::Interface, "Interface"},
};

struct JoosType {
  std::set<Modifier> modifiers;
  Type type;
  std::string identifier;
  std::unique_ptr<AST::Node> astNode;
  JoosBody declare;
  JoosType(std::set<Modifier> modifiers, Type type, std::string identifier,
           std::unique_ptr<AST::Node> astNode = nullptr);
  std::unordered_set<JoosType *> subType;
};
std::ostream &operator<<(std::ostream &stream, const JoosType &joosType);

class JoosTypeVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  std::set<Modifier> getModifiers();
  std::string getIdentifier();
  Type getType();

private:
  void visitProperties(const AST::Node &node);
  std::set<Modifier> modifiers;
  Type type;
  std::string identifier;
};

}; // namespace Env

#endif // ENVJOOSTYPE_HPP
