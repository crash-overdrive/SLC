#ifndef WEEDER_HPP
#define WEEDER_HPP

#include "EnvTypeDeclaration.hpp"

namespace Weed {

class Weeder {
public:
  Weeder(const Env::TypeDeclaration &typeDeclaration, const std::string &fileName);
  bool verify();

private:
  const Env::TypeDeclaration &typeDeclaration;
  const std::string &fileName;

  bool verifyType();
  bool typeNameSameAsFileName();
  bool classNotAbstractAndFinal();

  bool verifyConstructors();
  bool classContainsConstructor();

  bool verifyMethods();
  bool interfaceMethodNotStaticNorFinalNorNative();
  bool methodBodyExistsIffNotAbstractNorNative();
  bool abstractMethodNotStaticNorFinal();
  bool staticMethodNotFinal();
  bool nativeMethodIsStatic();

  bool verifyFields();
  bool fieldNotFinal();

  bool verifyLiterals();
  bool integerLiteralInRange();

  // functions below are not needed as they are handled by the parser
  // bool interfaceNoFieldNorConstructor();
  // bool interfaceMethodNoBody();
  // bool voidOnlyAsReturnTypeOfMethod();
  // bool formalParameterOfMethodHasNoInitializer();
  // bool noMultiDimensionalArrayTypeOrCreationExpression();
  // bool noExplicitThisOrSuperCallsInMethodOrConstructors();
};

} // namespace Weed

#endif
