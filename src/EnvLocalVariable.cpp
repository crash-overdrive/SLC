#include "ASTVisitorUtil.hpp"
#include "EnvLocalVariable.hpp"

namespace Env {

std::map<std::string, VariableDescriptor> Environment::getVariableTable() {
  return variableTable;
}

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

LocalVariableAnalysis::LocalVariableAnalysis(bool log) : log(log) {
  addEnvironment();
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
    std::cerr << "ERROR!! Variable: " << name << " with descriptor: " << variableDescriptor << " could not be added" << "\n";
    return false;
  }
  environments.back().addVariable(name, variableDescriptor);
  if (log) {
    std::cerr << "Added: Variable: " << name << " with descriptor: " << variableDescriptor << "\n";
  }
  return true;
}

void LocalVariableAnalysis::addEnvironment() {
  Env::Environment environment;
  environments.push_back(environment);
  if (log) {
    std::cerr << "Added: New Environment" << "\n";
  }
}

void LocalVariableAnalysis::removeEnvironment() {
  if (log) {
    auto &environment = environments.back();
    for (auto const &variable : environment.getVariableTable()) {
      std::cerr << "Removed: Variable: " << variable.first << " with variable descriptor: " << variable.second << "\n";
    }
    std::cerr << "Removed: Environment" << "\n";
  }
  environments.pop_back();
}

JoosLocalVariableVisitor::JoosLocalVariableVisitor(bool log) :
  localVariableAnalysis(Env::LocalVariableAnalysis(log)) {};

void JoosLocalVariableVisitor::visit(const AST::SingleVariableDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);
  if (!localVariableAnalysis.addVariable(propertiesVisitor.getIdentifier(), propertiesVisitor.getVariableDescriptor())) {
    setError();
    return;
  }
}

void JoosLocalVariableVisitor::visit(const AST::Block &block) {
  localVariableAnalysis.addEnvironment();
  dispatchChildren(block);
  localVariableAnalysis.removeEnvironment();
}

}; // namespace Env
