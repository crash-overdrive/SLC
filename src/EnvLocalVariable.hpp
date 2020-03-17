#ifndef LOCALVARIABLESCOPE_HPP
#define LOCALVARIABLESCOPE_HPP

#include <map>
#include <stack>

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"

namespace Env {

class Environment {
public:
  bool findVariable(const std::string &name);
  bool addVariable(const std::string &name, Env::VariableDescriptor variableDescriptor);

private:
  std::map<std::string, Env::VariableDescriptor> variableTable;
};

class LocalVariableAnalysis {
public:
  bool findVariable(const std::string &name);
  bool addVariable(const std::string &name, Env::VariableDescriptor variableDescriptor);
  void addEnvironment();
  void removeEnvironment();
private:
  std::vector <Environment> environments;
};

class JoosMethodBodyVisitor : public AST::TrackVisitor {

}

}; // namespace Env

#endif
