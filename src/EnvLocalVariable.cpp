#include "EnvLocalVariable.hpp"

namespace Env {

LocalVariableScope::LocalVariableScope(LocalVariableScope *parent)
    : parent(parent) {}

LocalVariableScope *LocalVariableScope::pop() const { return parent; }

LocalVariableScope *LocalVariableScope::push() {
  children.emplace_back(std::make_unique<LocalVariableScope>(this));
  return children.back().get();
}

bool LocalVariableScope::lookUp(const std::string &name) {
  LocalVariableScope *node = this;
  while (node) {
    auto it = node->variables.find(name);
    if (it != node->variables.end()) {
      return true;
    }
    node = node->parent;
  }
  return false;
}

bool LocalVariableScope::add(const std::string &name) {
  if (lookUp(name)) {
    return false;
  }
  variables.emplace(name);
  return true;
}

}; // namespace Env
