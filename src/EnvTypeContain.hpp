#ifndef ENVTYPECONTAIN_HPP
#define ENVTYPECONTAIN_HPP

#include "EnvTypeBody.hpp"

namespace Env {

bool validSignature(const Method *derived, const Method *base);
bool validReplace(const Method *derived, const Method *base);
bool validFinal(const Method *base);
bool validDerivedStatic(const Method *derived, const Method *base);
bool validBaseStatic(const Method *derived, const Method *base);
bool validModifier(const Method *derived, const Method *base);
bool isReplace(const Method *derived, const Method *base);

class ClassVTable {
public:
  ClassVTable();
  const Method *findMethod(const std::string &identifier,
                           const std::vector<Type> &args) const;
  const Field *findField(const std::string &identifier) const;
  std::vector<Method *> getMethods() const;
  bool update(Method *method);
  void update(Field *field);
  bool hasAbstract() const;
  void addVTable();
  void updateOffset();

private:
  struct VTable {
    std::vector<Field *> fields;
    std::vector<Method *> methods;
  };
  std::vector<VTable> vTables;
};

class Selector {
public:
  Selector(const Method *signature, const Method *implement = nullptr);
  const Method *getSignature() const;
  const Method *getImplements() const;
  bool setImplement(const Method *implement);

private:
  const Method *signature;
  const Method *implement;
};

class TypeContain {
public:
  std::string vtablelabel;
  std::string selectorlabel;
  void inheritContain(TypeContain &contain);
  bool mergeContain(const TypeContain &contain);
  bool mergeInterfaceMethod(const Method *base);

  void addDeclare(Field *field);
  bool addDeclare(Method *method);

  const Field *findField(const std::string &identifier) const;
  const Method *findMethod(const std::string &identifier,
                           const std::vector<Type> &args) const;
  bool hasAbstract() const;
  void updateOffset();
  std::vector<Method *> getMethods() const;
  std::vector<Field *> getFields() const;

private:
  void updateObjField(Field *field);
  const Field *findObjField(const std::string &identifier) const;
  Selector *findSelector(const std::string &identifier,
                         const std::vector<Type> &args);

  ClassVTable classVTable;
  std::vector<Selector> selectors;
  std::vector<Field *> objFields;
};

} // namespace Env

#endif // ENVTYPECONTAIN_HPP
