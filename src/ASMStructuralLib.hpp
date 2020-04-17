#ifndef ASM_STRUCTURAL_LIB_H
#define ASM_STRUCTURAL_LIB_H

#include <iostream>
#include <string>

namespace ASM {

class AnonymousLabelService;

class StructuralLib {
  AnonymousLabelService &label;

public:
  StructuralLib(AnonymousLabelService &label);
  std::string IfThenElse(const std::string &condition, const std::string &s1,
                         const std::string &s2);
  std::string WhileLoop(const std::string &condition,
                        const std::string &statement);
  std::string ForLoop(const std::string &init, const std::string &condition,
                      const std::string &increment,
                      const std::string &statement);
};

std::ostream &printDataSection(std::ostream &ostream);
std::ostream &printLabel(std::ostream &ostream, const std::string &label);
std::ostream &printExit(std::ostream &ostream);

} // namespace ASM

#endif
