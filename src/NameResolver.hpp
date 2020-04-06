#ifndef NAMERESOLVER_HPP
#define NAMERESOLVER_HPP

#include "EnvLocal.hpp"

namespace Name {

class Resolver {
public:
  Resolver(const Env::Local &local, const Env::TypeLink &typeLink);

  std::optional<Env::Type>
  findField(const std::vector<std::string> &name) const;
  std::optional<Env::Type> findField(Env::Type type,
                                     const std::string &identifier) const;

  std::optional<Env::Type> findMethod(const std::vector<std::string> &name,
                                      const std::vector<Env::Type> &args) const;
  std::optional<Env::Type> findMethod(Env::Type type,
                                      const std::string &identifier,
                                      const std::vector<Env::Type> &args) const;

  std::optional<Env::Type>
  findConstructor(Env::Type type, const std::vector<Env::Type> &args) const;

private:
  template <class InputIt>
  std::optional<Env::Type> findField(Env::Type type, InputIt first,
                                     InputIt last) const;
  template <class InputIt>
  std::optional<Env::Type> findMethod(Env::Type type,
                                      const std::vector<Env::Type> &args,
                                      InputIt first, InputIt last) const;

  std::optional<Env::Type> matchObject(const std::string &identifier) const;
  template <class T>
  bool isVisible(const Env::TypeDeclaration *other, T t) const;
  template <class T> bool isStatic(T t) const;

  const Env::Local &local;
  const Env::TypeLink &typeLink;
  const Env::TypeDeclaration &decl;
};

} // namespace Name

#endif // NAMERESOLVER_HPP
