#include "CodeGenStart.hpp"
#include "ASMStructuralLib.hpp"
#include <filesystem>

namespace CodeGen {

void prepareOutput() { std::filesystem::create_directory(outputDirectory); }

std::string getASMFile(const std::string &fullName) {
  std::string fileName = fullName.substr(fullName.find_last_of('/') + 1);
  fileName.erase(fileName.find("."));
  return outputDirectory + '/' + fileName + ".asm";
}

StartGenerator::StartGenerator(std::ostream &ostream) : ostream(ostream) {}

void StartGenerator::generateHeader() {
  ASM::printTextSection(ostream);
  ASM::printLabel(ostream, "_start");
}

void StartGenerator::generateStaticInit(const Env::TypeBody &body) {
  if (!entry) {
    for (const auto &method : body.getMethods()) {
      if (isEntry(method)) {
        entry = &method;
        return;
      }
    }
  }
}

bool StartGenerator::isEntry(const Env::Method &method) {
  return method.modifiers.find(Env::Modifier::Static) !=
             method.modifiers.end() &&
         method.returnType == Env::TypeKeyword::Integer &&
         method.identifier == "test" && method.args.empty();
}

void StartGenerator::generateEntry() {
  ostream << "mov eax, 123\n";
  ASM::printExit(ostream);
}

} // namespace CodeGen
