#include "EnvLocalVariable.hpp"

namespace Env {

LocalVariableScope::LocalVariableScope(LocalVariableScope *Parent)
    : Parent(Parent) {}

LocalVariableScope *LocalVariableScope::pop() const { return Parent; }

LocalVariableScope *LocalVariableScope::push() {
  Children.emplace_back(std::make_unique<LocalVariableScope>(this));
  return Children.back().get();
}

bool LocalVariableScope::lookUp(const std::string &Name) {
  LocalVariableScope *Node = this;
  while (Node) {
    auto it = Node->Variables.find(Name);
    if (it != Node->Variables.end()) {
      return true;
    }
    Node = Node->Parent;
  }
  return false;
}

bool LocalVariableScope::add(const std::string &Name) {
  if (lookUp(Name)) {
    return false;
  }
  Variables.emplace(Name);
  return true;
}

}; // namespace Env
