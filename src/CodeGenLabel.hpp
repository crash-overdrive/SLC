#ifndef CODEGENLABEL_HPP
#define CODEGENLABEL_HPP

#include "EnvTypeContain.hpp"

namespace CodeGen {

class LabelGenerator {
public:
  LabelGenerator(std::vector<std::string> path);
  void generateConstructor(Env::Constructor &constructor);
  void generateStaticFieldInit(Env::TypeBody &body);
  void generateFieldInit(Env::TypeBody &body);
  void generateMethod(Env::Method &method);
  void generateVTable(Env::ClassVTable &vTable);
  void generateSelector(Env::Selector &selector);

private:
  std::vector<std::string> path;
};

} // namespace CodeGen

#endif // CODEGENLABEL_HPP
