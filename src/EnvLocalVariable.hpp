#ifndef ENVLOCALVARIABLE_HPP
#define ENVLOCALVARIABLE_HPP

#include "ASTVisitor.hpp"
#include "EnvTypeLink.hpp"
#include <map>
#include <vector>

namespace Env {

class VariableTable {
public:
  Type *findVariable(const std::string &name);
  bool addVariable(const std::string &name, Type type);

private:
  friend std::ostream &operator<<(std::ostream &stream,
                                  const VariableTable &table);
  std::map<std::string, Type> variableMap;
};

std::ostream &operator<<(std::ostream &stream, const VariableTable &table);

class LocalVariableAnalysis {
public:
  LocalVariableAnalysis(bool log = false);
  Type *findVariable(const std::string &name);
  bool addVariable(const std::string &name, Type type);
  void addVariableTable();
  void removeVariableTable();

private:
  bool log;
  std::vector<VariableTable> tables;
};

class LocalVariableVisitor : public AST::TrackVisitor {
public:
  LocalVariableVisitor(const TypeLink &typeLink, bool log = false);
  void visit(const AST::SingleVariableDeclaration &decl) override;
  void visit(const AST::Block &block) override;
  void visit(const AST::SimpleType &simpleType) override;

private:
  const TypeLink &typeLink;
  LocalVariableAnalysis localVariableAnalysis;
};

} // namespace Env

#endif // ENVLOCALVARIABLE_HPP
