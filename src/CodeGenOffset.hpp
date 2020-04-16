#ifndef CODEGENOFFSET_HPP
#define CODEGENOFFSET_HPP

#include "EnvTypeContain.hpp"

class InterfaceOffsetGenerator {
public:
  void generateInterfaceMethod(Env::Method &method);

private:
  std::vector<Env::Method> interfaceMethods;
};

} // namespace CodeGen

#endif // CODEGENOFFSET_HPP
