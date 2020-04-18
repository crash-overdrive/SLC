#include "ASMStructuralLib.hpp"
#include "ASMServices.hpp"

namespace ASM {

StructuralLib::StructuralLib(AnonymousLabelService &label) : label(label) {}

std::string StructuralLib::IfThenElse(const std::string &condition,
                                      const std::string &s1,
                                      const std::string &s2) {
  std::string s2label = label.getUniqueLabel();
  std::string end = label.getUniqueLabel();
  std::string cmpNjmp = "cmp eax, 0;\nje " + s2label + "\n";
  std::string jmp = "jmp " + end + "\n";
  return condition + cmpNjmp + s1 + jmp + s2label + ":\n" + s2 + end + ":\n";
}

std::string StructuralLib::WhileLoop(const std::string &condition,
                                     const std::string &statement) {
  std::string beg = label.getUniqueLabel();
  std::string end = label.getUniqueLabel();

  std::string cmpNjmp = "cmp eax, 0;\nje " + end + "\n";
  std::string jmp = "jmp " + beg + "\n";

  return beg + ":\n" + condition + cmpNjmp + statement + jmp + end + ":\n";
}

std::string StructuralLib::ForLoop(const std::string &init,
                                   const std::string &condition,
                                   const std::string &increment,
                                   const std::string &statement) {
  std::string beg = label.getUniqueLabel();
  std::string end = label.getUniqueLabel();

  std::string cmpNjmp = "cmp eax, 0;\nje " + end + "\n";
  std::string jmp = "jmp " + beg + "\n";

  return init + beg + ":\n" + condition + cmpNjmp + statement + increment +
         jmp + end + ":\n";
}

std::ostream &printCall(std::ostream &ostream, const std::string &identifier) {
  ostream << "extern " << identifier << '\n';
  ostream << "call " << identifier << '\n';
  return ostream;
}

std::ostream &printProlog(std::ostream &ostream) {
  ostream << "push ebp\n";
  ostream << "mov ebp, esp\n";
  return ostream;
}

std::ostream &printEpilogue(std::ostream &ostream) {
  ostream << "mov esp, ebp\n";
  ostream << "pop ebp\n";
  ostream << "ret\n";
  return ostream;
}

std::ostream &printAssignment(std::ostream &ostream) {
  ostream << "pop ebx\n";
  ostream << "mov [ebx], eax\n";
  return ostream;
}

std::ostream &printLocalVariable(std::ostream &ostream, off_t offset) {
  ostream << "mov eax, ebp\n";
  ostream << "add eax, " << offset << '\n';
  return ostream;
}

std::ostream &printDataSection(std::ostream &ostream) {
  ostream << "section .data\n";
  return ostream;
}

std::ostream &printTextSection(std::ostream &ostream) {
  ostream << "section .text\n";
  return ostream;
}

std::ostream &printLabel(std::ostream &ostream, const std::string &label) {
  ostream << "global " << label << '\n';
  ostream << label << ":\n";
  return ostream;
}

std::ostream &printExit(std::ostream &ostream) {
  ostream << "mov ebx, eax\n";
  ostream << "mov eax, 1\n";
  ostream << "int 0x80\n";
  return ostream;
}

} // namespace ASM
