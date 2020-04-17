#include "EnvTypeBody.hpp"
#include "Weeder.hpp"

namespace Weed {

Weeder::Weeder(Env::TypeDeclaration &typeDeclaration, std::string &fileName)
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

bool Weeder::verifyConstructors() { return classContainsConstructor() &&
                                          constructorHasCorrectIdentifier(); }

bool Weeder::classContainsConstructor() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Class) {
    size_t numberOfConstructors = typeDeclaration.body.getConstructors().size();
    if (numberOfConstructors == 0) {
      std::cerr
          << "Class should have atleast one explicit constructor, in file: "
          << fileName << '\n';
      return false;
    }
  }
  return true;
}

bool Weeder::constructorHasCorrectIdentifier() {
  for (const auto &constructor : typeDeclaration.body.getConstructors()) {
    if (constructor.identifier != typeDeclaration.identifier) {
      std::cerr << "Constructor has incorrect identifier, expected: " <<
                typeDeclaration.identifier << ", got: " << constructor.identifier << "\n";
      return false;
    }
  }
  return true;
}

bool Weeder::verifyMethods() {
  return methodIsNotPackagePrivate() &&
        interfaceMethodNotStaticNorFinalNorNative() &&
        methodBodyExistsIffNotAbstractNorNative() &&
        abstractMethodNotStaticNorFinal() &&
        staticMethodNotFinal() &&
        nativeMethodIsStatic();
}

bool Weeder::methodIsNotPackagePrivate() {
  std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
  for (auto const &method : methods) {
    std::set<Env::Modifier> methodModifiers = method.modifiers;
    bool isPublic = (methodModifiers.find(Env::Modifier::Public) !=
                       methodModifiers.end());
    bool isProtected = (methodModifiers.find(Env::Modifier::Protected) !=
                       methodModifiers.end());
    if (!(isPublic || isProtected)) {
      std::cerr << "Method can not be package private, method: " <<
      method.identifier << ", in file: " << fileName << '\n';
      return false;
    }
  }
  return true;
}

bool Weeder::interfaceMethodNotStaticNorFinalNorNative() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Interface) {
    std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
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

bool Weeder::methodBodyExistsIffNotAbstractNorNative() {
  if (typeDeclaration.keyword == Env::DeclarationKeyword::Class) {
    std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
    for (auto const &method : methods) {
      std::set<Env::Modifier> methodModifiers = method.modifiers;
      bool isAbstract =
          (methodModifiers.find(Env::Modifier::Abstract) != methodModifiers.end());
      bool isNative =
          (methodModifiers.find(Env::Modifier::Native) != methodModifiers.end());

      MethodBodyVisitor visitor;
      method.astNode->accept(visitor);
      bool isBodyPresent = visitor.isBlockPresentInMethod();

      if (isAbstract == false && isNative == false) {
        if (isBodyPresent == false) {
          std::cerr << "Non-Abstract/Non-Native methods must have a body, method: "
                    << method.identifier << ", in file: " << fileName << '\n';
          return false;
        }
      }
      else { // method is either Abstract or Native
        if (isBodyPresent == true) {
          std::cerr << "Abstract/Native methods can not have a body, method: "
                    << method.identifier << ", in file: " << fileName << '\n';
          return false;
        }
      }
    }
  }
  return true;
}

bool Weeder::abstractMethodNotStaticNorFinal() {
  std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
  for (auto const &method : methods) {
    std::set<Env::Modifier> methodModifiers = method.modifiers;
    bool isAbstract = (methodModifiers.find(Env::Modifier::Abstract) !=
                       methodModifiers.end());
    bool isStatic = (methodModifiers.find(Env::Modifier::Static) !=
                      methodModifiers.end());
    bool isFinal = (methodModifiers.find(Env::Modifier::Final) !=
                      methodModifiers.end());
    if (isAbstract) {
      if (isStatic || isFinal) {
        std::cerr << "Abstract Method can not be static or final, method: " <<
        method.identifier << ", in file: " << fileName << '\n';
        return false;
      }
    }
  }
  return true;
}

bool Weeder::staticMethodNotFinal() {
  std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
  for (auto const &method : methods) {
    std::set<Env::Modifier> methodModifiers = method.modifiers;
    bool isStatic = (methodModifiers.find(Env::Modifier::Static) !=
                      methodModifiers.end());
    bool isFinal = (methodModifiers.find(Env::Modifier::Final) !=
                      methodModifiers.end());
    if (isStatic) {
      if (isFinal) {
        std::cerr << "Static Method can not be final, method: " <<
        method.identifier << ", in file: " << fileName << '\n';
        return false;
      }
    }
  }
  return true;
}

bool Weeder::nativeMethodIsStatic() {
  std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
  for (auto const &method : methods) {
    std::set<Env::Modifier> methodModifiers = method.modifiers;
    bool isNative = (methodModifiers.find(Env::Modifier::Native) !=
                      methodModifiers.end());
    bool isStatic = (methodModifiers.find(Env::Modifier::Static) !=
                      methodModifiers.end());
    if (isNative) {
      if (!isStatic) {
        std::cerr << "Native Method must be static, method: " <<
        method.identifier << ", in file: " << fileName << '\n';
        return false;
      }
    }
  }
  return true;
}

bool Weeder::verifyFields() {
  return fieldNotFinal();
}

bool Weeder::fieldNotFinal() {
  const std::vector<Env::Field> &fields = typeDeclaration.body.getFields();
  for (auto const &field : fields) {
    std::set<Env::Modifier> fieldModifiers = field.modifiers;
    bool isFinal =
        (fieldModifiers.find(Env::Modifier::Final) != fieldModifiers.end());
    if (isFinal) {
        std::cerr << "Field can not be final, field: "
                  << field.identifier << ", in file: " << fileName << '\n';
        return false;
    }
  }
  return true;
}

bool Weeder::verifyLiterals() { return true; }

bool Weeder::integerLiteralInRange() {
  for (const auto &constructor : typeDeclaration.body.getConstructors()) {
    LiteralVisitor visitor;
    constructor.astNode->accept(visitor);
    if (visitor.isErrorState()) {
      std::cerr << "Integer Literal out of range in constructor: " <<
        constructor.identifier << ", in file: " << fileName << '\n';
      return false;
    }
  }

  std::vector<Env::Method> methods = typeDeclaration.body.getMethods();
  for (auto const &method : methods) {
    LiteralVisitor visitor;
    method.astNode->accept(visitor);
    if (visitor.isErrorState()) {
      std::cerr << "Integer Literal out of range in method: " <<
        method.identifier << ", in file: " << fileName << '\n';
      return false;
    }
  }

  return true;
}

MethodBodyVisitor::MethodBodyVisitor() : isBlockFound(false) {}

void MethodBodyVisitor::visit(const AST::MethodDeclaration &methodDeclaration) {
  dispatchChildren(methodDeclaration);
}

void MethodBodyVisitor::visit(const AST::Block &block) {
  (void)block;
  isBlockFound = true;
}

bool MethodBodyVisitor::isBlockPresentInMethod() {
  return isBlockFound;
}

void LiteralVisitor::visit(const AST::DecIntLiteral &decIntLiteral) {
  std::string stringLiteral = decIntLiteral.getLiteral();
  long long literal = std::stoll(stringLiteral);

  if (literal >= 2147483648) {
    std::cerr << literal << " is out of int range\n";
    setError();
  }
}

} // namespace Weed
