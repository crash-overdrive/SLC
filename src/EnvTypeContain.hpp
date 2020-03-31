#ifndef ENVTYPECONTAIN
#define ENVTYPECONTAIN

#include "EnvTypeBody.hpp"

namespace Env {

class TypeContain {
public:
  bool inheritMethod(const Method *method);
  bool addDeclareMethod(const Method *method);

  bool inheritField(const Field *field);
  bool addDeclareField(const Field *field);

  bool mergeContain(const TypeContain &contain);

  const Field *findField(const std::string &identifier) const;
  const Method *findMethod(const std::string &identifier,
                           const std::vector<Type> &args) const;
  bool hasAbstract() const;

private:
  static bool validReplace(const Method *derived, const Method *base);
  static bool validSignature(const Method *derived, const Method *base);
  static bool validFinal(const Method *base);
  static bool validDerivedStatic(const Method *derived, const Method *base);
  static bool validBaseStatic(const Method *derived, const Method *base);
  static bool validModifier(const Method *derived, const Method *base);
  static bool isReplace(const Method *derived, const Method *base);
  std::vector<const Method *> methods;
  std::vector<const Field *> fields;
};

} // namespace Env

#endif // ENVTYPECONTAIN
