#ifndef ENVJOOSCONTAIN
#define ENVJOOSCONTAIN

#include "EnvJoosBody.hpp"

namespace Env {

class JoosContain {
public:
  bool inheritMethod(const JoosMethod *method);
  bool addDeclareMethod(const JoosMethod *method);

  bool inheritField(const JoosField *field);
  bool addDeclareField(const JoosField *field);

  bool mergeContain(const JoosContain &contain);

  const JoosField *findField(const VariableDescriptor &variableDescriptor,
                             const std::string &identifier) const;
  const JoosMethod *
  findMethod(const std::string &identifier,
             const std::vector<VariableDescriptor> &args) const;
  bool hasAbstract() const;

private:
  static bool validReplace(const JoosMethod *derived, const JoosMethod *base);
  static bool validSignature(const JoosMethod *derived, const JoosMethod *base);
  static bool validFinal(const JoosMethod *base);
  static bool validDerivedStatic(const JoosMethod *derived,
                                 const JoosMethod *base);
  static bool validBaseStatic(const JoosMethod *derived,
                              const JoosMethod *base);
  static bool validModifier(const JoosMethod *derived, const JoosMethod *base);
  static bool isReplace(const JoosMethod *derived, const JoosMethod *base);
  std::vector<const JoosMethod *> methods;
  std::vector<const JoosField *> fields;
};

} // namespace Env

#endif // ENVJOOSCONTAIN
