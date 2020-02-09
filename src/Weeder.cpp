#include "Weeder.hpp"

bool Weed::AbstractFinalClass(const Parse::Tree &Tree) {
  (void)Tree;
  return true;
}

const std::vector<Weed::Check> Weed::JoosChecks{
  AbstractFinalClass,
};
