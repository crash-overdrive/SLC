#ifndef ENVSCOPE_HPP
#define ENVSCOPE_HPP

#include "ASTVisitor.hpp"
#include <map>
#include <string>

namespace Env {

class Scope {
public:
  enum Type { GLOBAL, PACKAGE, CLASS, INTERFACE };
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
  void setRoot(Scope &Scope) const;
  bool error() const;

private:
  Scope *Current;
  bool errorState;
};

} // namespace Env

#endif // ENVSCOPE_HPP
