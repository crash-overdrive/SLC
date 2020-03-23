#include "EnvJoosContain.hpp"
#include <algorithm>

namespace Env {

void JoosContain::addMethod(const JoosMethod &method) {
  methods.emplace_back(&method);
}

bool JoosContain::inheritMethod(const JoosMethod *method) {
  auto it = std::find_if(methods.begin(), methods.end(),
                         [&](const JoosMethod *containMethod) {
                           return *containMethod == *method;
                         });
  if (it == methods.end()) {
    methods.emplace_back(method);
    return true;
  }
  return replaceSignature(*it, method) && replaceFinal(method) &&
         replaceStatic(*it, method) && replaceModifier(*it, method);
}

bool JoosContain::replaceSignature(const JoosMethod *base,
                                   const JoosMethod *parent) {
  return base->returnType == parent->returnType;
}

bool JoosContain::replaceFinal(const JoosMethod *parent) {
  return parent->modifiers.find(Modifier::Final) == parent->modifiers.end();
}

bool JoosContain::replaceStatic(const JoosMethod *base,
                                const JoosMethod *parent) {
  return !(base->modifiers.find(Modifier::Static) == base->modifiers.end() &&
           parent->modifiers.find(Modifier::Static) != parent->modifiers.end());
}

bool JoosContain::replaceModifier(const JoosMethod *base,
                                  const JoosMethod *parent) {
  return !(base->modifiers.find(Modifier::Protected) != base->modifiers.end() &&
           parent->modifiers.find(Modifier::Public) != parent->modifiers.end());
}

void JoosContain::addField(const JoosField &field) {
  fields.emplace_back(&field);
}

bool JoosContain::inheritField(const JoosField *field) {
  fields.emplace_back(field);
  return true;
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
