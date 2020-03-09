#ifndef ASTVISITORUTIL_HPP
#define ASTVISITORUTIL_HPP

#include "ASTVisitor.hpp"
#include <set>

namespace AST {

class NameVisitor : public TrackVisitor {
public:
  void visit(const Name &name) override;
  void visit(const Identifier &Identifier) override;
  std::vector<std::string> getName();

private:
  std::vector<std::string> Name{};
};

class PropertiesVisitor : public AST::Visitor {
public:
  void visit(const AST::FieldDeclaration &Decl) override;
  void visit(const AST::MethodDeclaration &Decl) override;
  void visit(const AST::ConstructorDeclaration &Decl) override;
  void visit(const AST::ClassDeclaration &Decl) override;
  void visit(const AST::InterfaceDeclaration &Decl) override;

  void visit(const AST::Modifier &Modifer) override;
  void visit(const AST::SimpleType &SimpleType) override;
  void visit(const AST::PrimitiveType &SimpleType) override;
  void visit(const AST::Identifier &Identifier) override;
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
