#include "CodeGenDeclaration.hpp"
#include "ASMStructuralLib.hpp"
#include "CodeGenVisitor.hpp"

namespace CodeGen {

DeclarationGenerator::DeclarationGenerator(std::ostream &ostream,
                                           const Env::TypeLink &typeLink)
    : ostream(ostream), typeLink(typeLink) {}

void DeclarationGenerator::generateBody(const Env::TypeBody &body) {
  ASM::printTextSection(ostream);
  generateGlobal(body);
  for (const auto &constructor : body.getConstructors()) {
    generateConstructor(constructor);
  }

  for (const auto &method : body.getMethods()) {
    if (method.modifiers.find(Env::Modifier::Abstract) !=
        method.modifiers.end()) {
      continue;
    }
    generateMethod(method);
  }
}

void DeclarationGenerator::generateGlobal(const Env::TypeBody &body) {
  for (const auto &constructor : body.getConstructors()) {
    ASM::printGlobal(ostream, constructor.label);
  }

  for (const auto &method : body.getMethods()) {
    if (method.modifiers.find(Env::Modifier::Abstract) ==
        method.modifiers.end()) {
      ASM::printGlobal(ostream, method.label);
    }
  }
}

void DeclarationGenerator::generateMethod(const Env::Method &method) {
  CodeGen::FrameStackVisitor frameVisitor;
  method.astNode->accept(frameVisitor);

  ASM::printLabel(ostream, method.label);
  ASM::printProlog(ostream, (unsigned int)(frameVisitor.getDeclaration() -
                                           method.args.size()));
  CodeGen::Visitor visitor(ostream, typeLink);
  if (method.modifiers.find(Env::Modifier::Static) == method.modifiers.end()) {
    visitor.setOffset(8 + 4 * (off_t)method.args.size());
  }
  method.astNode->accept(visitor);
  ASM::printEpilogue(ostream);
}

void DeclarationGenerator::generateConstructor(
    const Env::Constructor &constructor) {
  ASM::printLabel(ostream, constructor.label);
}

void DeclarationGenerator::generateContain(const Env::TypeContain &contain) {
  ASM::printLabel(ostream, contain.vtablelabel);
  for (const auto &method : contain.getMethods()) {
    if (method->modifiers.find(Env::Modifier::Abstract) ==
        method->modifiers.end()) {
      ASM::printDeclare(ostream, method->label);
    }
  }
}

} // namespace CodeGen
