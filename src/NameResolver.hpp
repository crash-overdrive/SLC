#ifndef NAMERESOLVER_HPP
#define NAMERESOLVER_HPP

#include "EnvLocal.hpp"

namespace Name {

class ResolverListener {
public:
  virtual ~ResolverListener() = default;
  virtual void listenLocal();
  virtual void listenField(const Env::Field &field);
  virtual void listenStaticField(const Env::Field &field);
  virtual void listenMethod(const Env::Method &method);
  virtual void listenStaticMethod(const Env::Method &method);
  virtual void listenConstructor(const Env::Constructor &constructor);
  virtual void listenArrayLength();
};

static ResolverListener nullResolverListener = ResolverListener();

class FieldResolver {
public:
  FieldResolver(const Env::Local &local, const Env::TypeLink &typeLink,
                ResolverListener &listener);

  std::optional<Env::Type> match(const std::vector<std::string> &name);

  std::optional<Env::Type> match(Env::Type type, const std::string &identifier);
  std::vector<std::string>::const_iterator getFirst() const;

private:
  void matchLocal();
  void matchField();
  void matchStatic();
  void matchIdentifier(const std::string &identifier);
  void matchName();

  std::vector<std::string>::const_iterator first;
  std::vector<std::string>::const_iterator last;
  std::optional<Env::Type> type;
  const Env::Local &local;
  const Env::TypeLink &typeLink;
  ResolverListener &listener;
};

class MethodResolver {
public:
  MethodResolver(const Env::Local &local, const Env::TypeLink &typeLink,
                 ResolverListener &listener);
  std::optional<Env::Type> match(const std::vector<std::string> &name);
  std::optional<Env::Type> match(Env::Type type, const std::string &identifier);
  void setArgs(std::vector<Env::Type> args);

private:
  void matchImplicit();
  void matchField();
  void matchStaticMethod();
  void matchIdentifier(const std::string &identifier);

  std::vector<std::string>::const_iterator first;
  std::vector<std::string>::const_iterator last;
  std::optional<Env::Type> type;
  std::vector<Env::Type> args;
  const Env::Local &local;
  const Env::TypeLink &typeLink;
  ResolverListener &listener;
};

class ConstructorResolver {
public:
  ConstructorResolver(const Env::TypeLink &typeLink,
                      ResolverListener &listener);

  std::optional<Env::Type> match(Env::Type type,
                                 const std::vector<Env::Type> &args);

private:
  const Env::TypeLink &typeLink;
  ResolverListener &listener;
};

class ResolverFactory {
public:
  ResolverFactory(const Env::Local &local, const Env::TypeLink &typeLink,
                  ResolverListener &listener = nullResolverListener);
  FieldResolver getField();
  MethodResolver getMethod();
  ConstructorResolver getConstructor();

private:
  const Env::Local &local;
  const Env::TypeLink &typeLink;
  ResolverListener &listener;
};

template <class T>
bool isInstanceVisible(const Env::TypeLink &typeLink,
                       const Env::TypeDeclaration *other, T t);

template <class T> bool isStaticVisible(const Env::TypeLink &typeLink, T t);

} // namespace Name

#endif // NAMERESOLVER_HPP
