#ifndef CODEGENOFFSET_HPP
#define CODEGENOFFSET_HPP

#include "EnvTypeContain.hpp"

namespace CodeGen {

class InterfaceOffsetGenerator {
public:
  void update(Env::Method &method);
  size_t getSize() const;

private:
  std::vector<std::reference_wrapper<Env::Method>> baseMethods;
  off_t offset = 0;
};

} // namespace CodeGen

#endif // CODEGENOFFSET_HPP
