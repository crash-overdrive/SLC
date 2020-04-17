#ifndef CODEGENDECLARATION_HPP
#define CODEGENDECLARATION_HPP

#include "EnvTypeContain.hpp"
#include <iostream>

namespace CodeGen {

class DeclarationGenerator {
public:
  DeclarationGenerator(std::ostream &ostream, const Env::TypeLink &typeLink);
  void generateBody(const Env::TypeBody &body);
  void generateConstructor(const Env::Constructor &constructor);
  void generateStaticField(const Env::TypeBody &body);
  void generateField(const Env::TypeBody &body);
  void generateMethod(const Env::Method &method);

  void generateContain(const Env::TypeContain &contain);
  void generateVTable(const Env::ClassVTable &vTable);
  void generateSelector(const Env::Selector &selector);

private:
  std::ostream &ostream;
  const Env::TypeLink &typeLink;
};

} // namespace CodeGen

#endif // CODEGENDECLARATION_HPP
