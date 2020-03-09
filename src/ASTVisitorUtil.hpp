#ifndef ASTVISITORUTIL_HPP
#define ASTVISITORUTIL_HPP

#include "ASTVisitor.hpp"
#include <set>

namespace AST {

class NameVisitor : public Visitor {
public:
  void visit(const Name &name) override;
  void visit(const Identifier &Identifier) override;
  std::vector<std::string> getName();

private:
  std::vector<std::string> Name{};
};

class PropertiesVisitor : public Visitor {
public:
  void visit(const Modifier &Modifer) override;
  void visit(const SimpleType &SimpleType) override;
  void visit(const PrimitiveType &SimpleType) override;
  void visit(const Identifier &Identifier) override;
  std::set<ModifierCode> getModifiers();
  std::string getType();
  std::string getName();

private:
  std::set<ModifierCode> Modifiers{};
  std::string Type;
  std::string Name;
};

} // namespace AST

#endif // ASTVISITORUTIL_HPP
