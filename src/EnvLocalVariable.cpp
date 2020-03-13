#include "EnvLocalVariable.hpp"

namespace Env {

bool Environment::findVariable(const std::string &name) {
  return (variableTable.find(name) != variableTable.end());
}

bool Environment::addVariable(const std::string &name, Env::VariableDescriptor variableDescriptor) {
  if (findVariable(name)) {
    return false;
  }
  variableTable[name] = variableDescriptor;
  return true;
}

bool LocalVariableAnalysis::findVariable(const std::string &name) {
  for (auto &environment : environments) {
    if (environment.findVariable(name)) {
      return true;
    }
  }
  return false;
}

bool LocalVariableAnalysis::addVariable(const std::string &name, Env::VariableDescriptor variableDescriptor) {
  if (findVariable(name)) {
    return false;
  }
  environments.back().addVariable(name, variableDescriptor);
  return true;
}

void LocalVariableAnalysis::addEnvironment() {
  Env::Environment environment;
  environments.push_back(environment);
}

void LocalVariableAnalysis::removeEnvironment() {
  environments.pop_back();
}

}; // namespace Env
