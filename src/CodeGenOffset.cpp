#include "CodeGenOffset.hpp"

namespace CodeGen {

void InterfaceOffsetGenerator::update(Env::TypeBody &body) {
  for (auto &method : body.getMethods()) {
    update(method);
  }
}

void InterfaceOffsetGenerator::update(Env::Method &method) {
  for (auto &base : baseMethods) {
    if (base.get() == method) {
      method.offset = base.get().offset;
      return;
    }
  }
  method.offset = offset;
  offset += 4;
  baseMethods.emplace_back(method);
}

size_t InterfaceOffsetGenerator::getSize() const { return baseMethods.size(); }

} // namespace CodeGen
