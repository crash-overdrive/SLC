#ifndef ENVSCOPE_HPP
#define ENVSCOPE_HPP

#include "ASTVisitor.hpp"
#include <map>
#include <string>

namespace Env {

class Scope {
public:
  enum Type { GLOBAL, PACKAGE, CLASS, INTERFACE };

  Scope(Type type);
  Scope(const std::string &name, Type type);
  Scope *update(const std::string &name, Type type);
  Scope *find(const std::string &name);
  const std::string &getName() const;
  Type getType() const;

private:
  Scope *updatePackage(const std::string &name, Type type);
  Scope *addType(const std::string &name, Type type);
  std::string name;
  Type type;
  std::map<std::string, Scope> children;
};

class ScopeBuilder : public AST::Visitor {
public:
  ScopeBuilder();
  void visit(const AST::Start &Start) override;
  void visit(const AST::PackageDeclaration &Decl) override;
  void visit(const AST::ClassDeclaration &Decl) override;
  void visit(const AST::Identifier &Identifier) override;

  void setRoot(Scope &Scope);
  bool error() const;

private:
  Scope *Current;
  bool errorState;
  Scope::Type type;
};

} // namespace Env

#endif // ENVSCOPE_HPP
