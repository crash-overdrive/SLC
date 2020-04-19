#include "CodeGenStart.hpp"
#include "ASMStructuralLib.hpp"
#include <algorithm>
#include <filesystem>

namespace CodeGen {

void prepareOutput() { std::filesystem::create_directory(outputDirectory); }

std::string getASMFile(const std::string &fullName) {
  std::string fileName = fullName.substr(fullName.find_last_of('/') + 1);
  fileName.erase(fileName.find("."));
  return outputDirectory + '/' + fileName + ".s";
}

StartGenerator::StartGenerator(std::ostream &ostream) : ostream(ostream) {}

void StartGenerator::generateHeader() {
  ASM::printTextSection(ostream);
  ASM::printGlobal(ostream, "_start");
  ASM::printLabel(ostream, "_start");
}

void StartGenerator::generateStaticInit(const Env::TypeBody &body) {
  if (!entry) {
    auto it = std::find_if(body.getMethods().begin(), body.getMethods().end(),
                           isEntry);
    if (it != body.getMethods().end()) {
      entry = &(*it);
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
  if (entry) {
    ASM::printProlog(ostream, 0);
    ASM::printCall(ostream, entry->label);
  } else {
    ostream << "mov eax, 1\n";
  }
  ASM::printExit(ostream);
  ASM::printEpilogue(ostream);
}

} // namespace CodeGen
