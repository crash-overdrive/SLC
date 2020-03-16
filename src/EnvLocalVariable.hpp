#ifndef LOCALVARIABLESCOPE_HPP
#define LOCALVARIABLESCOPE_HPP

#include <memory>
#include <unordered_set>
#include <vector>

namespace Env {

class LocalVariableScope {
public:
  LocalVariableScope(LocalVariableScope *parent = nullptr);
  LocalVariableScope *pop() const;
  LocalVariableScope *push();
  bool lookUp(const std::string &name);
  bool add(const std::string &name);

private:
  LocalVariableScope *parent;
  std::unordered_set<std::string> variables;
  std::vector<std::unique_ptr<LocalVariableScope>> children;
};

}; // namespace Env

#endif
