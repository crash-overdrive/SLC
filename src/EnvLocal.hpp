#ifndef ENVLOCAL_HPP
#define ENVLOCAL_HPP

#include "ASTVisitor.hpp"
#include "EnvTypeLink.hpp"
#include <map>
#include <vector>

namespace Env {

struct Variable {
  std::string identifier;
  Type type;
  off_t offset;
  Variable() = default;
  Variable(std::string identifier, Type type);
};

class VariableTable {
public:
  VariableTable(bool args = false);
  std::optional<Variable> findVariable(const std::string &identifier) const;
  bool addVariable(std::string identifier, Type type);
  VariableTable createNextVariableTable() const;

private:
  friend std::ostream &operator<<(std::ostream &stream,
                                  const VariableTable &table);
  std::map<std::string, Variable> variableMap;
  off_t offset = -4;
  bool args;
};

std::ostream &operator<<(std::ostream &stream, const VariableTable &table);

class Local {
public:
  Local(bool log = false);
  std::optional<Variable> findVariable(const std::string &identifier) const;
  bool addVariable(std::string name, Type type);
  void addVariableTable();
  void removeVariableTable();
  const Variable &getLastVariable() const;

private:
  bool log;
  std::vector<VariableTable> tables;
  Variable lastVariable;
};

class LocalTrackVisitor : public AST::TrackVisitor {
public:
  LocalTrackVisitor(const TypeLink &typeLink, bool log = false);
  void visit(const AST::SingleVariableDeclaration &decl) override;
  void visit(const AST::Block &block) override;
  void visit(const AST::SimpleType &simpleType) override;

protected:
  const Local &getLocal() const;

private:
  const TypeLink &typeLink;
  Local local;
};

} // namespace Env

#endif // ENVLOCAL_HPP
