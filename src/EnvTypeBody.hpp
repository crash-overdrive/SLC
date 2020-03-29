#ifndef ENVTYPEBODY_HPP
#define ENVTYPEBODY_HPP

#include "ASTVisitor.hpp"
#include "EnvType.hpp"
#include <set>
#include <vector>

namespace Env {

enum class Modifier {
  Public,
  Protected,
  Static,
  Abstract,
  Final,
  Native,
};

const std::unordered_map<std::string, Modifier> stringModifier{
    {"PUBLIC", Modifier::Public}, {"PROTECTED", Modifier::Protected},
    {"STATIC", Modifier::Static}, {"ABSTRACT", Modifier::Abstract},
    {"FINAL", Modifier::Final},   {"NATIVE", Modifier::Native},
};

const std::unordered_map<Modifier, std::string> modifierString{
    {Modifier::Public, "PUBLIC"}, {Modifier::Protected, "PROTECTED"},
    {Modifier::Static, "STATIC"}, {Modifier::Abstract, "ABSTRACT"},
    {Modifier::Final, "FINAL"},   {Modifier::Native, "NATIVE"},
};

struct Field {
  std::set<Modifier> modifiers;
  Type type;
  std::string identifier;
  const AST::FieldDeclaration *astNode;
  Field() = default;
  Field(std::set<Modifier> modifiers, Type type, std::string identifier,
        const AST::FieldDeclaration *astNode);
  bool operator==(const Field &field) const;
  bool operator!=(const Field &field) const;
};
std::ostream &operator<<(std::ostream &stream, const Field &field);

struct Method {
  std::set<Modifier> modifiers;
  Type returnType;
  std::string identifier;
  std::vector<Type> args;
  const AST::MethodDeclaration *astNode;
  Method() = default;
  Method(std::set<Modifier> modifiers, Type returnType, std::string identifier,
         std::vector<Type> args, const AST::MethodDeclaration *astNode);
  bool operator==(const Method &method) const;
  bool operator!=(const Method &method) const;
};
std::ostream &operator<<(std::ostream &stream, const Method &method);

struct Constructor {
  std::set<Modifier> modifiers;
  std::string identifier;
  std::vector<Type> args;
  const AST::ConstructorDeclaration *astNode;
  Constructor(std::set<Modifier> modifiers, std::string identifier,
              std::vector<Type> args,
              const AST::ConstructorDeclaration *astNode);
  bool operator==(const Constructor &constructor) const;
  bool operator!=(const Constructor &constructor) const;
};
std::ostream &operator<<(std::ostream &stream, const Constructor &constructor);

class TypeBody {
public:
  const std::vector<Field> &getFields() const;
  bool addField(Field field);

  const std::vector<Method> &getMethods() const;
  bool addMethod(Method method);

  const std::vector<Constructor> &getConstructors() const;
  bool addConstructor(Constructor constructor);

  void setAbstract();

private:
  friend std::ostream &operator<<(std::ostream &stream, const TypeBody &body);
  std::vector<Method> methods;
  std::vector<Field> fields;
  std::vector<Constructor> constructors;
};
std::ostream &operator<<(std::ostream &stream, const TypeBody &body);

class TypeLink;

class TypeBodyVisitor : public AST::Visitor {
public:
  TypeBodyVisitor(const TypeLink &typeLink);
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  void visit(const AST::FieldDeclaration &decl) override;
  void visit(const AST::MethodDeclaration &decl) override;
  void visit(const AST::ConstructorDeclaration &decl) override;
  std::vector<Field> getFields();
  std::vector<Method> getMethods();
  std::vector<Constructor> getConstructors();

private:
  const TypeLink &typeLink;
  std::vector<Field> fields;
  std::vector<Method> methods;
  std::vector<Constructor> constructors;
};

} // namespace Env

#endif // ENVTYPEBODY_HPP
