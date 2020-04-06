#ifndef ASTVISITORUTIL_HPP
#define ASTVISITORUTIL_HPP

#include "ASTVisitor.hpp"
#include "EnvTypeBody.hpp"

namespace AST {

class NameVisitor : public Visitor {
public:
  void visit(const Name &name) override;
  void visit(const Identifier &identifier) override;
  std::vector<std::string> getName();

private:
  std::vector<std::string> name;
};

class PropertiesVisitor : public Visitor {
public:
  void visit(const Modifier &modifer) override;
  void visit(const Identifier &identifier) override;
  std::set<Env::Modifier> getModifiers(); // gets all the modifiers
  std::string getIdentifier();

private:
  std::set<Env::Modifier> modifiers;
  std::string identifier;
};

class TypeVisitor : public Visitor {
public:
  TypeVisitor(const Env::TypeLink &typeLink);
  void visit(const AST::PrimitiveType &primitiveType) override;
  void visit(const AST::SimpleType &simpleType) override;
  void visit(const AST::ArrayType &arrayType) override;
  Env::Type getType();

private:
  const Env::TypeLink &typeLink;
  Env::Type type;
};

class ArgumentsDeclarationVisitor : public Visitor {
public:
  ArgumentsDeclarationVisitor(const Env::TypeLink &typeLink);
  void visit(const SingleVariableDeclaration &decl) override;
  std::vector<Env::Type> getArgs();

private:
  const Env::TypeLink &typeLink;
  std::vector<Env::Type> args;
};

} // namespace AST

#endif // ASTVISITORUTIL_HPP
