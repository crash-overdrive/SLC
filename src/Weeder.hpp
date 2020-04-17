#ifndef WEEDER_HPP
#define WEEDER_HPP

#include "EnvTypeDeclaration.hpp"

namespace Weed {

class Weeder {
public:
  Weeder(Env::TypeDeclaration &typeDeclaration, std::string &fileName);
  bool verify();

private:
  Env::TypeDeclaration &typeDeclaration;
  std::string &fileName;

  bool verifyType();
  bool typeNameSameAsFileName();
  bool classNotAbstractAndFinal();

  bool verifyConstructors();
  bool classContainsConstructor();
  bool constructorHasCorrectIdentifier();

  bool verifyMethods();
  bool methodIsNotPackagePrivate();
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

class MethodBodyVisitor : public AST::Visitor {
public:
  MethodBodyVisitor();
  void visit(const AST::MethodDeclaration &methodDeclaration) override;
  void visit(const AST::Block &block) override;
  bool isBlockPresentInMethod();
private:
  bool isBlockFound;
};

class LiteralVisitor : public AST::TrackVisitor {
public:
  void visit(const AST::DecIntLiteral &decIntLiteral) override;
};

} // namespace Weed

#endif
