#include "ASMServices.hpp"
#include <algorithm>
#include <sstream>

namespace ASM {

AnonymousLabelService::AnonymousLabelService(const std::string &prefix)
    : prefix(prefix), instanceCount(0) {}

int AnonymousLabelService::getUniqueId() { return instanceCount++; }

std::string AnonymousLabelService::getUniqueLabel() {
  std::ostringstream str;
  str << prefix << instanceCount++;
  return str.str();
}

} // namespace ASM
