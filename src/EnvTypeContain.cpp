#include "EnvTypeContain.hpp"
#include <algorithm>

namespace Env {

bool TypeContain::inheritMethod(const Method *method) {
  const Method *derived = findMethod(method->identifier, method->args);
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

bool TypeContain::addDeclareMethod(const Method *method) {
  for (auto it = methods.begin(); it != methods.end();) {
    const Method *base = *it;
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

bool TypeContain::inheritField(const Field *field) {
  fields.emplace_back(field);
  return true;
}

bool TypeContain::addDeclareField(const Field *field) {
  fields.emplace_back(field);
  return true;
}

bool TypeContain::validReplace(const Method *derived, const Method *base) {
  return validFinal(base) && validBaseStatic(derived, base) &&
         validDerivedStatic(derived, base) && validModifier(derived, base);
}

bool TypeContain::validSignature(const Method *derived, const Method *base) {
  return derived->returnType == base->returnType;
}

bool TypeContain::validFinal(const Method *base) {
  return base->modifiers.find(Modifier::Final) == base->modifiers.end();
}

bool TypeContain::validDerivedStatic(const Method *derived,
                                     const Method *base) {
  return !(derived->modifiers.find(Modifier::Static) ==
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) != base->modifiers.end());
}

bool TypeContain::validBaseStatic(const Method *derived, const Method *base) {
  return !(derived->modifiers.find(Modifier::Static) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) == base->modifiers.end());
}

bool TypeContain::validModifier(const Method *derived, const Method *base) {
  return !(derived->modifiers.find(Modifier::Protected) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Public) != base->modifiers.end());
}

bool TypeContain::isReplace(const Method *derived, const Method *base) {
  return derived->modifiers.find(Modifier::Abstract) ==
             derived->modifiers.end() &&
         base->modifiers.find(Modifier::Abstract) != base->modifiers.end();
}

bool TypeContain::mergeContain(const TypeContain &contain) {
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

const Field *TypeContain::findField(const Type &type,
                                    const std::string &identifier) const {
  for (const auto &field : fields) {
    if (field->identifier == identifier && field->type == type) {
      return field;
    }
  }
  return nullptr;
}

const Method *TypeContain::findMethod(const std::string &identifier,
                                      const std::vector<Type> &args) const {
  for (const auto &method : methods) {
    if (method->identifier == identifier && method->args == args) {
      return method;
    }
  }
  return nullptr;
}

bool TypeContain::hasAbstract() const {
  for (auto const &method : methods) {
    if (method->modifiers.find(Modifier::Abstract) != method->modifiers.end()) {
      return true;
    }
  }
  return false;
}

} // namespace Env
