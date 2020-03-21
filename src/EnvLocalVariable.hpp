#ifndef LOCALVARIABLESCOPE_HPP
#define LOCALVARIABLESCOPE_HPP

#include <map>
#include <stack>

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"

namespace Env {

class Environment {
public:
  std::map<std::string, VariableDescriptor> getVariableTable();
  bool findVariable(const std::string &name);
  bool addVariable(const std::string &name, VariableDescriptor variableDescriptor);

private:
  std::map<std::string, VariableDescriptor> variableTable;
};

class LocalVariableAnalysis {
public:
  LocalVariableAnalysis(bool log);
  bool findVariable(const std::string &name);
  bool addVariable(const std::string &name, VariableDescriptor variableDescriptor);
  void addEnvironment();
  void removeEnvironment();
private:
  bool log;
  std::vector <Environment> environments;
};

class JoosLocalVariableVisitor : public AST::TrackVisitor {
public:
  JoosLocalVariableVisitor(bool log);
  void visit(const AST::SingleVariableDeclaration &decl) override;
  void visit(const AST::Block &block) override;
  LocalVariableAnalysis localVariableAnalysis;
};

}; // namespace Env

#endif
