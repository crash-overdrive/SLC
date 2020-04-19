#include "EnvTypeContain.hpp"
#include <algorithm>

namespace Env {

bool validSignature(const Method *derived, const Method *base) {
  return derived->returnType == base->returnType;
}

bool validReplace(const Method *derived, const Method *base) {
  return validFinal(base) && validBaseStatic(derived, base) &&
         validDerivedStatic(derived, base) && validModifier(derived, base);
}

bool validFinal(const Method *base) {
  return base->modifiers.find(Modifier::Final) == base->modifiers.end();
}

bool validDerivedStatic(const Method *derived, const Method *base) {
  return !(derived->modifiers.find(Modifier::Static) ==
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) != base->modifiers.end());
}

bool validBaseStatic(const Method *derived, const Method *base) {
  return !(derived->modifiers.find(Modifier::Static) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Static) == base->modifiers.end());
}

bool validModifier(const Method *derived, const Method *base) {
  return !(derived->modifiers.find(Modifier::Protected) !=
               derived->modifiers.end() &&
           base->modifiers.find(Modifier::Public) != base->modifiers.end());
}

bool isReplace(const Method *derived, const Method *base) {
  return derived->modifiers.find(Modifier::Abstract) ==
             derived->modifiers.end() &&
         base->modifiers.find(Modifier::Abstract) != base->modifiers.end();
}

ClassVTable::ClassVTable() { addVTable(); }

const Method *ClassVTable::findMethod(const std::string &identifier,
                                      const std::vector<Type> &args) const {
  for (const auto &vTable : vTables) {
    for (const auto &method : vTable.methods) {
      if (method->identifier == identifier && method->args == args) {
        return method;
      }
    }
  }
  return nullptr;
}

const Field *ClassVTable::findField(const std::string &identifier) const {
  for (const auto &vTable : vTables) {
    for (const auto &field : vTable.fields) {
      if (field->identifier == identifier) {
        return field;
      }
    }
  }
  return nullptr;
}

std::vector<Method *> ClassVTable::getMethods() const {
  std::vector<Method *> methods;
  for (const auto &vTable : vTables) {
    methods.insert(methods.end(), vTable.methods.begin(), vTable.methods.end());
  }
  return methods;
}

bool ClassVTable::update(Method *method) {
  for (auto &vTable : vTables) {
    for (auto &base : vTable.methods) {
      if (*base != *method) {
        continue;
      }
      if (!validSignature(method, base) || !validReplace(method, base)) {
        return false;
      }
      base = method;
      return true;
    }
  }
  vTables.back().methods.emplace_back(method);
  return true;
}

void ClassVTable::update(Field *field) {
  for (auto &vTable : vTables) {
    for (auto &base : vTable.fields) {
      if (*base == *field) {
        base = field;
        return;
      }
    }
  }
  vTables.back().fields.emplace_back(field);
}

bool ClassVTable::hasAbstract() const {
  for (const auto &vTable : vTables) {
    for (const auto &method : vTable.methods) {
      if (method->modifiers.find(Modifier::Abstract) !=
          method->modifiers.end()) {
        return true;
      }
    }
  }
  return false;
}

void ClassVTable::addVTable() { vTables.emplace_back(); }

void ClassVTable::updateOffset() {
  off_t offset = 0;
  for (auto &vTable : vTables) {
    for (auto &method : vTable.methods) {
      method->offset = offset;
      offset += 4;
    }
  }
}

Selector::Selector(const Method *signature, const Method *implement)
    : signature(signature), implement(implement) {}

const Method *Selector::getSignature() const { return signature; }

const Method *Selector::getImplements() const { return implement; }

bool Selector::setImplement(const Method *implement) {
  if (!validSignature(implement, signature) ||
      !validReplace(implement, signature)) {
    return false;
  }
  if (isReplace(implement, signature)) {
    this->implement = implement;
  }
  return true;
}

void TypeContain::inheritContain(TypeContain &contain) {
  classVTable = contain.classVTable;
  selectors = contain.selectors;
  objFields = contain.objFields;
  classVTable.addVTable();
}

bool TypeContain::mergeContain(const TypeContain &contain) {
  for (const auto &selector : contain.selectors) {
    if (!mergeInterfaceMethod(selector.getSignature())) {
      return false;
    }
  }
  return true;
}

bool TypeContain::mergeInterfaceMethod(const Method *base) {
  Selector *found = findSelector(base->identifier, base->args);
  if (found) {
    return validSignature(found->getSignature(), base);
  }
  auto &selector = selectors.emplace_back(base);
  const Method *method = classVTable.findMethod(base->identifier, base->args);
  if (!method) {
    return true;
  }
  if (!validSignature(method, base)) {
    return false;
  }
  if (isReplace(method, base)) {
    return selector.setImplement(method);
  }
  return true;
}

bool TypeContain::addDeclare(Method *method) {
  if (!classVTable.update(method)) {
    return false;
  }
  Selector *selector = findSelector(method->identifier, method->args);
  if (selector && !selector->setImplement(method)) {
    return false;
  }
  return true;
}

void TypeContain::addDeclare(Field *field) {
  if (field->modifiers.find(Modifier::Static) != field->modifiers.end()) {
    classVTable.update(field);
  } else {
    updateObjField(field);
  }
}

const Field *TypeContain::findField(const std::string &identifier) const {
  const Field *field = classVTable.findField(identifier);
  if (field) {
    return field;
  }
  return findObjField(identifier);
}

const Method *TypeContain::findMethod(const std::string &identifier,
                                      const std::vector<Type> &args) const {
  const Method *method = classVTable.findMethod(identifier, args);
  if (method) {
    return method;
  }
  for (auto &selector : selectors) {
    const Method *method = selector.getSignature();
    if (method->identifier == identifier && method->args == args) {
      return method;
    }
  }
  return nullptr;
}

bool TypeContain::hasAbstract() const {
  for (const auto &selector : selectors) {
    if (!selector.getImplements()) {
      return true;
    }
  }
  return classVTable.hasAbstract();
}

void TypeContain::updateOffset() {
  classVTable.updateOffset();
  off_t offset = 4;
  for (auto &objField : objFields) {
    objField->offset = offset;
    offset += 4;
  }
}

std::vector<Method *> TypeContain::getMethods() const {
  return classVTable.getMethods();
}

std::vector<Field *> TypeContain::getFields() const { return objFields; }

void TypeContain::updateObjField(Field *field) {
  for (auto &objField : objFields) {
    if (*objField == *field) {
      objField = field;
    }
  }
  objFields.emplace_back(field);
}

const Field *TypeContain::findObjField(const std::string &identifier) const {
  for (const auto &objField : objFields) {
    if (objField->identifier == identifier) {
      return objField;
    }
  }
  return nullptr;
}

Selector *TypeContain::findSelector(const std::string &identifier,
                                    const std::vector<Type> &args) {
  for (auto &selector : selectors) {
    const Method *method = selector.getSignature();
    if (method->identifier == identifier && method->args == args) {
      return &selector;
    }
  }
  return nullptr;
}

} // namespace Env
