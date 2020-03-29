#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "ASTVisitor.hpp"
#include "EnvTypeBody.hpp"
#include <unordered_set>

namespace Env {

class HierarchyGraph;

class Hierarchy {
public:
  virtual ~Hierarchy() = default;
  virtual void buildSubType() const = 0;
  virtual bool buildContains() const = 0;
  virtual bool setBaseObject(const TypeDeclaration *base) = 0;
};

class InterfaceHierarchy : public Hierarchy {
public:
  explicit InterfaceHierarchy(TypeDeclaration &decl);
  bool addExtends(const TypeDeclaration *decl);

  bool setBaseObject(const TypeDeclaration *base) override;
  void buildSubType() const override;
  bool buildContains() const override;

private:
  friend HierarchyGraph;
  // Hard Coding getClass to filter getClass override in due to
  // Java specs ambiguity
  static bool isGetClass(const Method &method);
  TypeDeclaration &decl;
  std::unordered_set<const TypeDeclaration *> extends;
};

class ClassHierarchy : public Hierarchy {
public:
  explicit ClassHierarchy(TypeDeclaration &decl);
  bool setExtends(const TypeDeclaration *decl);
  bool addImplements(const TypeDeclaration *decl);

  bool setBaseObject(const TypeDeclaration *base) override;
  void buildSubType() const override;
  bool buildContains() const override;

private:
  friend HierarchyGraph;
  TypeDeclaration &decl;
  const TypeDeclaration *extends = nullptr;
  std::unordered_set<const TypeDeclaration *> implements;
};

class HierarchyGraph {
public:
  void addClass(ClassHierarchy hierarchy);
  void addInterface(InterfaceHierarchy hierarchy);
  bool topologicalSort();
  void buildSubType() const;
  bool buildContains() const;

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

class HierarchyVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &node) override;
  void visit(const AST::ClassDeclaration &node) override;
  void visit(const AST::InterfaceDeclaration &node) override;
  void visit(const AST::Interfaces &node) override;
  void visit(const AST::Extensions &node) override;
  void visit(const AST::Super &node) override;
  std::vector<std::vector<std::string>> getInterfaces();
  std::vector<std::vector<std::string>> getExtensions();
  std::vector<std::string> getSuper();

private:
  std::vector<std::vector<std::string>> interfaces;
  std::vector<std::vector<std::string>> extensions;
  std::vector<std::string> super;
};

} // namespace Env

#endif // ENVHIERARCHY_HPP
