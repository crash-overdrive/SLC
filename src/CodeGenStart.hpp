#ifndef CODEGENSTART_HPP
#define CODEGENSTART_HPP

#include "EnvTypeBody.hpp"
#include <iostream>

namespace CodeGen {

const std::string outputDirectory = "output";
const std::string outputStart = outputDirectory + "/start.s";

void prepareOutput();
std::string getASMFile(const std::string &fullName);

class StartGenerator {
public:
  StartGenerator(std::ostream &ostream);
  void generateHeader();
  void generateStaticInit(const Env::TypeBody &body);
  void generateEntry();

private:
  static bool isEntry(const Env::Method &method);
  const Env::Method *entry = nullptr;
  std::ostream &ostream;
};

} // namespace CodeGen

#endif // CODEGENSTART_HPP
