#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "EnvFileHeader.hpp"
#include <list>
#include <string>
#include <unordered_set>

namespace Env {

class TypeLink;
class HierarchyGraph;

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
  friend HierarchyGraph;
  std::unordered_set<Hierarchy *> extends;
};

class ClassHierarchy : public Hierarchy {
public:
  explicit ClassHierarchy(FileHeader header);
  bool setExtends(Hierarchy *hierarchy);
  bool addImplements(Hierarchy *hierarchy);

private:
  friend HierarchyGraph;
  Hierarchy *extends = nullptr;
  std::unordered_set<Hierarchy *> implements;
};

class HierarchyGraph {
public:
  InterfaceHierarchy &addInterface(FileHeader header);
  ClassHierarchy &addClass(FileHeader header);
  std::vector<std::unique_ptr<InterfaceHierarchy>> &getInterfaces();
  std::vector<std::unique_ptr<ClassHierarchy>> &getClasses();
  std::vector<Hierarchy *> &getHierarchies();

  bool topologicalSort();
  void buildSubType();
  bool buildContains();

private:
  struct DAGNode {
    Hierarchy *hierarchy;
    unsigned int inDegree = 0;
    std::vector<DAGNode *> outNodes;
    explicit DAGNode(Hierarchy *hierarchy);
    void addOutNode(DAGNode *node);
  };

  std::list<DAGNode> augmentGraph();
  std::vector<std::unique_ptr<InterfaceHierarchy>> interfaces;
  std::vector<std::unique_ptr<ClassHierarchy>> classes;
  std::vector<Env::Hierarchy *> hierarchies;
};

} // namespace Env

#endif // ENVHIERARCHY_HPP
