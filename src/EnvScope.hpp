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
  Scope *resolveName(const std::string &name);
  const std::string &getName() const;
  Type getType() const;

private:
  Scope *updatePackage(const std::string &name, Type type);
  Scope *addType(const std::string &name, Type type);
  std::string name;
  Type type;
  std::map<std::string, Scope> children;
};

} // namespace Env

#endif // ENVSCOPE_HPP
