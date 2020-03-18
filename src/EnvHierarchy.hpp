#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "EnvFileHeader.hpp"
#include <string>
#include <vector>

namespace Env {

class TypeLink;

class Hierarchy {
public:
  Hierarchy(FileHeader header);
  virtual ~Hierarchy() = default;

  const AST::Node *getASTNode() const;
  const std::string &getIdentifier() const;
  Type getType() const;

  const std::vector<std::string> &getPackage() const;
  void setPackage(std::vector<std::string> package);
  const TypeLink *getTypeLink() const;
  void setTypeLink(std::unique_ptr<TypeLink> typeLink);

private:
  FileHeader header;
  std::unique_ptr<TypeLink> typeLink;
  std::vector<std::string> package;
};

class InterfaceHierarchy : public Hierarchy {
public:
  explicit InterfaceHierarchy(FileHeader header);
  bool addExtends(Hierarchy *hierarchy);

private:
  std::vector<Hierarchy *> implements;
};

class ClassHierarchy : public Hierarchy {
public:
  explicit ClassHierarchy(FileHeader header);
  bool setExtends(Hierarchy *hierarchy);
  bool addImplements(Hierarchy *hierarchy);

private:
  Hierarchy *extends = nullptr;
  std::vector<Hierarchy *> implements;
};

class HierarchyBuilder {};

} // namespace Env

#endif // ENVHIERARCHY_HPP
