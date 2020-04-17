#include "Weeder.hpp"

namespace Weed {

Weeder::Weeder(const Env::TypeDeclaration &typeDeclaration, const std::string &fileName)
    : typeDeclaration(typeDeclaration), fileName(fileName) {}

bool Weeder::verify() {
  return verifyType() && verifyConstructors() && verifyMethods() &&
         verifyFields() && verifyLiterals();
}

bool Weeder::verifyType() {
  return typeNameSameAsFileName() && classNotAbstractAndFinal();
}

bool Weeder::typeNameSameAsFileName() {
  size_t startPosition = fileName.find_last_of('/') + 1;
  size_t endPosition = fileName.find_last_of(".") - startPosition;
  std::string name = fileName.substr(startPosition, endPosition);
  if (name != typeDeclaration.identifier) {
    std::cerr << "FileName does not match TypeName for file: " << fileName
              << '\n';
    return false;
  }
  return true;
}

bool Weeder::classNotAbstractAndFinal() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Class) {
    std::set<Env::Modifier> typeModifiers = typeDeclaration.modifiers;
    bool isAbstract =
        (typeModifiers.find(Env::Modifier::Abstract) != typeModifiers.end());
    bool isFinal =
        (typeModifiers.find(Env::Modifier::Final) != typeModifiers.end());
    if (isAbstract && isFinal) {
      std::cerr << "Class can not be both abstract and final in file: "
                << fileName << '\n';
      return false;
    }
  }
  return true;
}

bool Weeder::verifyConstructors() { return classContainsConstructor(); }

bool Weeder::classContainsConstructor() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Class) {
    int numberOfConstructors = joosType.declare.getConstructors().size();
    if (numberOfConstructors == 0) {
      std::cerr
          << "Class should have atleast one explicit constructor, in file: "
          << fileName << '\n';
      return false;
    }
  }
  return true;
}

bool Weeder::verifyMethods() {
  return interfaceMethodNotStaticNorFinalNorNative();
}

bool Weeder::interfaceMethodNotStaticNorFinalNorNative() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Interface) {
    std::vector<Env::JoosMethod> methods = joosType.declare.getMethods();
    for (auto const &method : methods) {
      std::set<Env::Modifier> methodModifiers = method.modifiers;
      bool isStatic = (methodModifiers.find(Env::Modifier::Static) !=
                       methodModifiers.end());
      bool isFinal =
          (methodModifiers.find(Env::Modifier::Final) != methodModifiers.end());
      bool isNative = (methodModifiers.find(Env::Modifier::Native) !=
                       methodModifiers.end());
      if (isFinal || isStatic || isNative) {
        std::cerr << "Interface methods can not be static, final or native in "
                     "method: "
                  << method.identifier << ", in file: " << fileName << '\n';
        return false;
      }
    }
  }
  return true;
}

bool Weeder::verifyFields() { return true; }

bool Weeder::verifyLiterals() { return true; }

} // namespace Weed
