#include "Weeder.hpp"

Parse::Weeder::Weeder() : Checks() {}

Parse::Weeder::Weeder(const std::vector<std::reference_wrapper<Check>> &Checks)
    : Checks(Checks) {}

void Parse::Weeder::addCheck(Check &Check) { Checks.emplace_back(Check); }

bool Parse::Weeder::verify(Tree &Tree) const {
  for (auto &Check : Checks) {
    if (!Check(Tree))
      return false;
  }
  return true;
}

const Parse::Check AbstractFinalClass = [](Parse::Tree &T) {
  (void)T;
  return true;
};

const std::vector<Parse::Check> JoosChecks{
    AbstractFinalClass,
};
