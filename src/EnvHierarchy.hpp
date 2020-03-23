#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "EnvJoosType.hpp"
#include <unordered_set>

namespace Env {

class HierarchyGraph;

class Hierarchy {
public:
  virtual ~Hierarchy() = default;
  virtual void buildSubType() const = 0;
  virtual void buildContains() const = 0;
};

class InterfaceHierarchy : public Hierarchy {
public:
  explicit InterfaceHierarchy(JoosType &joosType);
  bool addExtends(const JoosType *joosType);
  bool subType(const JoosType *joosType) const;
  void buildSubType() const override;
  void buildContains() const override;

private:
  friend HierarchyGraph;
  JoosType &joosType;
  std::unordered_set<const JoosType *> extends;
};

class ClassHierarchy : public Hierarchy {
public:
  explicit ClassHierarchy(JoosType &joosType);
  bool setExtends(const JoosType *joosType);
  bool addImplements(const JoosType *joosType);
  void buildSubType() const override;
  void buildContains() const override;

private:
  friend HierarchyGraph;
  JoosType &joosType;
  const JoosType *extends = nullptr;
  std::unordered_set<const JoosType *> implements;
};

class HierarchyGraph {
public:
  void addClass(ClassHierarchy hierarchy);
  void addInterface(InterfaceHierarchy hierarchy);
  bool topologicalSort();
  void buildSubType();
  bool buildContains();

private:
  struct DAGNode {
    unsigned int inDegree = 0;
    std::vector<DAGNode *> outNodes;
    Hierarchy &hierarchy;
    explicit DAGNode(Hierarchy &hierarchy);
    void addOutNode(DAGNode *node);
  };

  std::vector<DAGNode> augmentGraph();
  std::vector<InterfaceHierarchy> interfaces;
  std::vector<ClassHierarchy> classes;
  std::vector<Hierarchy *> order;
};

} // namespace Env

#endif // ENVHIERARCHY_HPP
