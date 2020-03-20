#ifndef ENVJOOSTYPE_HPP
#define ENVJOOSTYPE_HPP

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"
#include "EnvJoosBody.hpp"
#include <set>

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
  std::unique_ptr<AST::Node> node;
  std::set<Modifier> modifiers;
  Type type;
  std::string identifier;
  JoosBody declare;
  JoosBody contains;
};
std::ostream &operator<<(std::ostream &stream, const JoosType &joosType);

class JoosTypeVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  std::string getIdentifier();
  std::string getType();
  std::set<Env::Modifier> getModifiers();

private:
  void visitProperties(const AST::Node &node);
  std::set<Env::Modifier> modifiers;
  Type type;
  std::string identifier;
};

}; // namespace Env

#endif // ENVJOOSTYPE_HPP
