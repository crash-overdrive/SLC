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
  std::vector<std::string> getName();

private:
  std::vector<std::string> name;
};

class PropertiesVisitor : public Visitor {
public:
  void visit(const Modifier &modifer) override;
  void visit(const SimpleType &simpleType) override;
  void visit(const PrimitiveType &primitiveType) override;
  void visit(const Identifier &identifier) override;
  std::set<ModifierCode> getModifiers();
  std::string getName();

private:
  std::set<ModifierCode> modifiers;
  std::string name;
};

class TypeVisitor : public PropertiesVisitor {
public:
  Env::TypeDescriptor getTypeDescriptor();
private:
  Env::TypeDescriptor typeDescriptor;
};

class MethodVisitor : public PropertiesVisitor {
public:
  Env::VariableDescriptor getReturnType();
  std::vector<Env::VariableDescriptor> getArgs();
private:
  Env::VariableDescriptor returnType;
  std::vector<Env::VariableDescriptor> args;
};

class FieldVisitor : public PropertiesVisitor {
public:
  Env::VariableDescriptor getVariableDescriptor();
private:
  Env::VariableDescriptor variableDescriptor;
};

} // namespace AST

#endif // ASTVISITORUTIL_HPP
