#ifndef LOCALVARIABLESCOPE_HPP
#define LOCALVARIABLESCOPE_HPP

#include <memory>
#include <unordered_set>
#include <vector>

namespace Env {

class LocalVariableScope {
public:
  LocalVariableScope(LocalVariableScope *Parent = nullptr);
  LocalVariableScope *pop() const;
  LocalVariableScope *push();
  bool lookUp(const std::string &Name);
  bool add(const std::string &Name);

private:
  LocalVariableScope *Parent;
  std::unordered_set<std::string> Variables{};
  std::vector<std::unique_ptr<LocalVariableScope>> Children{};
};

}; // namespace Env

#endif