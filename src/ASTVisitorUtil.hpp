#ifndef ASTVISITORUTIL_HPP
#define ASTVISITORUTIL_HPP

#include <set>

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"

namespace AST {

class NameVisitor : public Visitor {
public:
  void visit(const Name &name) override;
  void visit(const Identifier &identifier) override;
  void visit(const PrimitiveType &primitiveType) override;
  std::vector<std::string> getName();

private:
  std::vector<std::string> name;
};

class PropertiesVisitor : public Visitor {
public:
  void visit(const Modifier &modifer) override;
  void visit(const PrimitiveType &primitiveType) override;
  void visit(const VoidType &voidType) override;
  void visit(const SimpleType &simpleType) override;
  void visit(const ArrayType &arrayType) override;
  void visit(const Identifier &identifier) override;
  std::set<ModifierCode> getModifiers(); // gets all the modifiers
  std::string getIdentifier();
  Env::VariableDescriptor getVariableDescriptor(); // gets VariableDescriptor for Primitive, Array and Simple Type

private:
  std::set<ModifierCode> modifiers;
  Env::VariableDescriptor variableDescriptor;
  std::string identifier;
};

class ArgumentsVisitor : public Visitor {
public:
  void visit(const SingleVariableDeclaration &decl) override;
  std::vector<Env::VariableDescriptor> getArgs();

private:
  std::vector<Env::VariableDescriptor> args;
};

} // namespace AST

#endif // ASTVISITORUTIL_HPP
