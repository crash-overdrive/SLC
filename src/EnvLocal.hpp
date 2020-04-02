#ifndef ENVLOCAL_HPP
#define ENVLOCAL_HPP

#include "ASTVisitor.hpp"
#include "EnvTypeLink.hpp"
#include <map>
#include <vector>

namespace Env {

class VariableTable {
public:
  std::optional<Type> findVariable(const std::string &name) const;
  bool addVariable(const std::string &name, Type type);

private:
  friend std::ostream &operator<<(std::ostream &stream,
                                  const VariableTable &table);
  std::map<std::string, Type> variableMap;
};

std::ostream &operator<<(std::ostream &stream, const VariableTable &table);

class Local {
public:
  Local(bool log = false);
  std::optional<Type> findVariable(const std::string &name) const;
  bool addVariable(const std::string &name, Type type);
  void addVariableTable();
  void removeVariableTable();

private:
  bool log;
  std::vector<VariableTable> tables;
};

class LocalVisitor : public AST::TrackVisitor {
public:
  LocalVisitor(const TypeLink &typeLink, bool log = false);
  void visit(const AST::SingleVariableDeclaration &decl) override;
  void visit(const AST::Block &block) override;
  void visit(const AST::SimpleType &simpleType) override;

private:
  const TypeLink &typeLink;
  Local local;
};

} // namespace Env

#endif // ENVLOCAL_HPP
