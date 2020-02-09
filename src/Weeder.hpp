#ifndef WEEDER_H
#define WEEDER_H

#include "ParseTree.hpp"
#include <functional>

namespace Parse {

using Check = std::function<bool(Parse::Tree &)>;

class Weeder {
public:
  Weeder();
  Weeder(const std::vector<std::reference_wrapper<Check>> &Checks);
  void addCheck(Check &Check);
  bool verify(Parse::Tree &Tree) const;

private:
  std::vector<std::reference_wrapper<Check>> Checks;
};

extern const Check AbstractFinalClass;

extern const std::vector<Check> JoosChecks;

} // namespace Parse

#endif // WEEDER_H
