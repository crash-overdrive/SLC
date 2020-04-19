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

std::ostream &printGlobal(std::ostream &ostream, const std::string &label) {
  ostream << "global " << label << '\n';
  return ostream;
}

std::ostream &printLabel(std::ostream &ostream, const std::string &label) {
  ostream << label << ":\n";
  return ostream;
}

std::ostream &printCall(std::ostream &ostream, const std::string &label) {
  ostream << "extern " << label << '\n';
  ostream << "call " << label << '\n';
  return ostream;
}

std::ostream &printProlog(std::ostream &ostream, unsigned int decl) {
  ostream << "push ebp\n";
  ostream << "mov ebp, esp\n";
  ostream << "sub esp, " << decl * 4 << '\n';
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

std::ostream &printExit(std::ostream &ostream) {
  ostream << "mov ebx, eax\n";
  ostream << "mov eax, 1\n";
  ostream << "int 0x80\n";
  return ostream;
}

std::ostream &printDeclare(std::ostream &ostream, const std::string &label) {
  ostream << "extern " << label << '\n';
  ostream << "dd " << label << '\n';
  return ostream;
}

std::ostream &printBinaryOperator(std::ostream &ostream,
                                  Type::BinaryOperator binaryOperator) {
  switch (binaryOperator) {
  case Type::BinaryOperator::Addition:
    ostream << "add eax, ebx\n";
    break;
  case Type::BinaryOperator::Subtraction:
    ostream << "sub ebx, eax\n";
    ostream << "mov eax, ebx\n";
    break;
  case Type::BinaryOperator::Multiplication:
    ostream << "imul eax, ebx\n";
    break;
  case Type::BinaryOperator::Division:
    ostream << "xor edx, edx\n";
    ostream << "xchg eax, ebx\n";
    ostream << "idiv ebx\n";
    break;
  case Type::BinaryOperator::Modulus:
    ostream << "xor edx, edx\n";
    ostream << "xchg ebx, eax\n";
    ostream << "idiv ebx\n";
    ostream << "mov eax, edx\n";
    break;
  case Type::BinaryOperator::Equal:
    ostream << "cmp ebx, eax\n";
    ostream << "sete al\n";
    break;
  case Type::BinaryOperator::NotEqual:
    ostream << "cmp ebx, eax\n";
    ostream << "setne al\n";
    break;
  case Type::BinaryOperator::LesserOrEqual:
    ostream << "cmp ebx, eax\n";
    ostream << "setle al\n";
    break;
  case Type::BinaryOperator::GreaterOrEqual:
    ostream << "cmp ebx, eax\n";
    ostream << "setge al\n";
    break;
  case Type::BinaryOperator::Lesser:
    ostream << "cmp ebx, eax\n";
    ostream << "setl al\n";
    break;
  case Type::BinaryOperator::Greater:
    ostream << "cmp ebx, eax\n";
    ostream << "setg al\n";
    break;
  case Type::BinaryOperator::BitwiseOr:
    ostream << "or eax, ebx\n";
    break;
  case Type::BinaryOperator::BitwiseAnd:
    ostream << "and eax, ebx\n";
    break;
  case Type::BinaryOperator::BitwiseXor:
    ostream << "xor eax, ebx\n";
    break;
  case Type::BinaryOperator::Or:
  case Type::BinaryOperator::And:
    break;
  }
  return ostream;
}

std::ostream &printUnaryOperator(std::ostream &ostream,
                                 Type::UnaryOperator unaryOperator) {
  switch (unaryOperator) {
  case Type::UnaryOperator::Not:
    ostream << "cmp eax, 0\n";
    ostream << "sete al\n";
    break;
  case Type::UnaryOperator::Negative:
    ostream << "neg eax\n";
    break;
  }
  return ostream;
}

} // namespace ASM
