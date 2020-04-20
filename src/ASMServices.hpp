#ifndef ASM_SERVICES_H
#define ASM_SERVICES_H

#include <string>
#include <vector>

namespace ASM {

class AnonymousLabelService {
public:
  AnonymousLabelService(const std::string &prefix = ".L");
  int getUniqueId();
  std::string getUniqueLabel();

private:
  const std::string prefix;
  int instanceCount;
};

} // namespace ASM

#endif
