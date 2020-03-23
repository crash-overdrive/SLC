#ifndef ENVJOOSCONTAIN
#define ENVJOOSCONTAIN

#include "EnvJoosBody.hpp"

namespace Env {

class JoosContain {
public:
  void addMethod(const JoosMethod &method);
  bool inheritMethod(const JoosMethod *method);

  void addField(const JoosField &field);
  bool inheritField(const JoosField *field);

  bool mergeContain(const JoosContain &contain);

  const JoosField *findField(const VariableDescriptor &variableDescriptor,
                             const std::string &identifier) const;
  const JoosMethod *
  findMethod(const std::string &identifier,
             const std::vector<VariableDescriptor> &args) const;

  bool hasAbstract() const;

private:
  static bool replaceSignature(const JoosMethod *base,
                               const JoosMethod *parent);
  static bool replaceFinal(const JoosMethod *parent);
  static bool replaceStatic(const JoosMethod *base, const JoosMethod *parent);
  static bool replaceModifier(const JoosMethod *base, const JoosMethod *parent);

  std::vector<const JoosMethod *> methods;
  std::vector<const JoosField *> fields;
};

} // namespace Env

#endif // ENVJOOSCONTAIN
