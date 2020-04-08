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
  void setUndefined();
  bool isUndefined(const std::string &name) const;
  Env::Type clearUndefined();

private:
  bool log;
  std::vector<VariableTable> tables;
  std::pair<std::string, Type> last;
  bool lastUndefined = false;
};

class LocalTrackVisitor : public AST::TrackVisitor {
public:
  LocalTrackVisitor(const TypeLink &typeLink, bool log = false);
  void visit(const AST::SingleVariableDeclaration &decl) override;
  void visit(const AST::Block &block) override;
  void visit(const AST::SimpleType &simpleType) override;

protected:
  Local &getLocal();

private:
  const TypeLink &typeLink;
  Local local;
};

} // namespace Env

#endif // ENVLOCAL_HPP
