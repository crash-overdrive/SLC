#include "EnvJoosContain.hpp"
#include <algorithm>

namespace Env {

bool JoosContain::inheritMethod(const JoosMethod *method) {
  const JoosMethod *derived = findMethod(method->identifier, method->args);
  if (!derived) {
    methods.emplace_back(method);
    return true;
  }
  if (!validSignature(derived, method)) {
    return false;
  }
  if (isReplace(derived, method)) {
    return validReplace(derived, method);
  }
  methods.emplace_back(method);
  return true;
}

bool JoosContain::addDeclareMethod(const JoosMethod *method) {
  for (auto it = methods.begin(); it != methods.end();) {
    const JoosMethod *base = *it;
    if (*base != *method) {
      ++it;
      continue;
    }
    if (!validSignature(method, base) || !validReplace(method, base)) {
      return false;
    }
    it = methods.erase(it);
  }
  methods.emplace_back(method);
  return true;
}

bool JoosContain::inheritField(const JoosField *field) {
  fields.emplace_back(field);
  return true;
}

bool JoosContain::addDeclareField(const JoosField *field) {
  fields.emplace_back(field);
  return true;
}

bool JoosContain::validReplace(const JoosMethod *derived,
                               const JoosMethod *base) {
  return validFinal(base) && validBaseStatic(derived, base) &&
         validDerivedStatic(derived, base) && validModifier(derived, base);
}

bool JoosContain::validSignature(const JoosMethod *derived,
                                 const JoosMethod *base) {
  return derived->returnType == base->returnType;
}

bool JoosContain::validFinal(const JoosMethod *base) {
  return base->modifiers.find(Modifier::Final) == base->modifiers.end();
}

bool JoosContain::validDerivedStatic(const JoosMethod *derived,
                                     const JoosMethod *base) {
  return !(derived->modifiers.find(Modifier::Static) ==
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) != base->modifiers.end());
}

bool JoosContain::validBaseStatic(const JoosMethod *derived,
                                  const JoosMethod *base) {
  return !(derived->modifiers.find(Modifier::Static) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) == base->modifiers.end());
}

bool JoosContain::validModifier(const JoosMethod *derived,
                                const JoosMethod *base) {
  return !(derived->modifiers.find(Modifier::Protected) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Public) != base->modifiers.end());
}

bool JoosContain::isReplace(const JoosMethod *derived, const JoosMethod *base) {
  return derived->modifiers.find(Modifier::Abstract) ==
             derived->modifiers.end() &&
         base->modifiers.find(Modifier::Abstract) != base->modifiers.end();
}

bool JoosContain::mergeContain(const JoosContain &contain) {
  for (const auto &field : contain.fields) {
    if (!inheritField(field)) {
      return false;
    }
  }
  for (const auto &method : contain.methods) {
    if (!inheritMethod(method)) {
      return false;
    }
  }
  return true;
}

const JoosField *
JoosContain::findField(const VariableDescriptor &variableDescriptor,
                       const std::string &identifier) const {
  for (const auto &field : fields) {
    if (field->identifier == identifier &&
        field->variableDescriptor == variableDescriptor) {
      return field;
    }
  }
  return nullptr;
}

const JoosMethod *
JoosContain::findMethod(const std::string &identifier,
                        const std::vector<VariableDescriptor> &args) const {
  for (const auto &method : methods) {
    if (method->identifier == identifier && method->args == args) {
      return method;
    }
  }
  return nullptr;
}

bool JoosContain::hasAbstract() const {
  for (auto const &method : methods) {
    if (method->modifiers.find(Modifier::Abstract) != method->modifiers.end()) {
      return true;
    }
  }
  return false;
}

} // namespace Env
