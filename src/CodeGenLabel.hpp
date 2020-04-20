#ifndef CODEGENLABEL_HPP
#define CODEGENLABEL_HPP

#include "EnvTypeContain.hpp"

namespace CodeGen {

class LabelGenerator {
public:
  LabelGenerator(std::vector<std::string> path);
  void generateDeclaration(Env::TypeDeclaration &decl);
  void generateConstructor(Env::Constructor &constructor);
  void generateMethod(Env::Method &method);
  void generateVTable(Env::TypeContain &contain);
  void generateSelector(Env::TypeContain &contain);

private:
  std::vector<std::string> path;
};

} // namespace CodeGen

#endif // CODEGENLABEL_HPP
