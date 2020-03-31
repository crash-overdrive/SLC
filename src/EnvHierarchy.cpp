#include "EnvHierarchy.hpp"
#include "ASTVisitorUtil.hpp"
#include "EnvTypeLink.hpp"
#include <algorithm>

namespace Env {

InterfaceHierarchy::InterfaceHierarchy(TypeDeclaration &decl) : decl(decl) {}

bool InterfaceHierarchy::addExtends(const TypeDeclaration *decl) {
  if (decl->keyword != DeclarationKeyword::Interface) {
    return false;
  }
  auto [it, flag] = extends.emplace(decl);
  return flag;
}

bool InterfaceHierarchy::setBaseObject(const TypeDeclaration *base) {
  if (extends.size()) {
    return true;
  }
  for (const auto &method : base->body.getMethods()) {
    // Skip getClass
    if (isGetClass(method)) {
      continue;
    }
    if (method.modifiers.find(Modifier::Public) != method.modifiers.end()) {
      Method baseMethod(method);
      baseMethod.modifiers = {Modifier::Public};
      if (!decl.body.addMethod(baseMethod)) {
        return false;
      }
    }
  }
  return true;
}

void InterfaceHierarchy::buildSubType() const {
  for (const auto &extend : extends) {
    decl.subType.insert(extend->subType.begin(), extend->subType.end());
  }
  decl.subType.emplace(&decl);
}

bool InterfaceHierarchy::buildContains() const {
  decl.body.setAbstract();
  for (const auto &extend : extends) {
    if (!decl.contain.mergeContain(extend->contain)) {
      return false;
    }
  }
  for (const auto &method : decl.body.getMethods()) {
    // Interface cannot override getclass method
    if (isGetClass(method) || !decl.contain.addDeclareMethod(&method)) {
      return false;
    }
  }
  return true;
}

bool InterfaceHierarchy::isGetClass(const Method &method) {
  return method.identifier == "getClass" && method.args.empty();
}

ClassHierarchy::ClassHierarchy(TypeDeclaration &decl) : decl(decl) {}

bool ClassHierarchy::setExtends(const TypeDeclaration *decl) {
  if (decl->keyword != DeclarationKeyword::Class ||
      decl->modifiers.find(Modifier::Final) != decl->modifiers.end()) {
    return false;
  }
  extends = decl;
  return true;
}

bool ClassHierarchy::addImplements(const TypeDeclaration *decl) {
  if (decl->keyword != DeclarationKeyword::Interface) {
    return false;
  }
  auto [it, flag] = implements.emplace(decl);
  return flag;
}

bool ClassHierarchy::setBaseObject(const TypeDeclaration *base) {
  if (!extends && base->astNode != decl.astNode) {
    extends = base;
  }
  return true;
}

void ClassHierarchy::buildSubType() const {
  if (extends) {
    decl.subType.insert(extends->subType.begin(), extends->subType.end());
  }
  for (const auto &implement : implements) {
    decl.subType.insert(implement->subType.begin(), implement->subType.end());
  }
  decl.subType.emplace(&decl);
}

bool ClassHierarchy::buildContains() const {
  if (extends && !decl.contain.mergeContain(extends->contain)) {
    return false;
  }
  for (const auto &implement : implements) {
    if (!decl.contain.mergeContain(implement->contain)) {
      return false;
    }
  }
  for (const auto &field : decl.body.getFields()) {
    decl.contain.addDeclareField(&field);
  }
  for (const auto &method : decl.body.getMethods()) {
    if (!decl.contain.addDeclareMethod(&method)) {
      return false;
    }
  }
  return !(decl.contain.hasAbstract() &&
           decl.modifiers.find(Modifier::Abstract) == decl.modifiers.end());
}

void HierarchyGraph::addClass(ClassHierarchy hierarchy) {
  classes.emplace_back(std::move(hierarchy));
}

void HierarchyGraph::addInterface(InterfaceHierarchy hierarchy) {
  interfaces.emplace_back(std::move(hierarchy));
}

bool HierarchyGraph::topologicalSort() {
  std::vector<DAGNode> nodes = augmentGraph();
  std::set<DAGNode *> workList;
  for (auto &node : nodes) {
    if (node.inDegree == 0) {
      workList.emplace(&node);
    }
  }
  while (workList.size()) {
    auto it = workList.begin();
    DAGNode *node = *it;
    order.emplace_back(&node->hierarchy);
    for (auto &outNode : node->outNodes) {
      --outNode->inDegree;
      if (!outNode->inDegree) {
        workList.emplace(outNode);
      }
    }
    workList.erase(it);
  }
  if (order.size() != interfaces.size() + classes.size()) {
    return false;
  }
  std::reverse(order.begin(), order.end());
  return true;
}

void HierarchyGraph::buildSubType() const {
  for (const auto &hierarchy : order) {
    hierarchy->buildSubType();
  }
}

bool HierarchyGraph::buildContains() const {
  for (const auto &hierarchy : order) {
    if (!hierarchy->buildContains()) {
      return false;
    }
  }
  return true;
}

HierarchyGraph::DAGNode::DAGNode(Hierarchy &hierarchy) : hierarchy(hierarchy) {}

void HierarchyGraph::DAGNode::addOutNode(DAGNode *node) {
  outNodes.emplace_back(node);
  ++node->inDegree;
}

std::vector<HierarchyGraph::DAGNode> HierarchyGraph::augmentGraph() {
  std::vector<DAGNode> nodes;
  nodes.reserve(interfaces.size() + classes.size());
  std::unordered_map<const TypeDeclaration *, DAGNode *> dagMap;
  for (auto &interfaceType : interfaces) {
    DAGNode &node = nodes.emplace_back(interfaceType);
    dagMap.emplace(&interfaceType.decl, &node);
  }
  for (auto &classType : classes) {
    DAGNode &node = nodes.emplace_back(classType);
    dagMap.emplace(&classType.decl, &node);
  }

  for (const auto &interfaceType : interfaces) {
    DAGNode *node = dagMap.at(&interfaceType.decl);
    for (const auto &extend : interfaceType.extends) {
      node->addOutNode(dagMap.at(extend));
    }
  }
  for (const auto &classType : classes) {
    DAGNode *node = dagMap.at(&classType.decl);
    if (classType.extends) {
      node->addOutNode(dagMap.at(classType.extends));
    }
    for (const auto &implement : classType.implements) {
      node->addOutNode(dagMap.at(implement));
    }
  }
  return nodes;
}

void HierarchyVisitor::visit(const AST::Start &node) { dispatchChildren(node); }

void HierarchyVisitor::visit(const AST::ClassDeclaration &node) {
  dispatchChildren(node);
}

void HierarchyVisitor::visit(const AST::InterfaceDeclaration &node) {
  dispatchChildren(node);
}

void HierarchyVisitor::visit(const AST::Interfaces &node) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(node);
  interfaces.emplace_back(visitor.getName());
}

void HierarchyVisitor::visit(const AST::Extensions &node) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(node);
  extensions.emplace_back(visitor.getName());
};

void HierarchyVisitor::visit(const AST::Super &node) {
  AST::NameVisitor visitor;
  visitor.dispatchChildren(node);
  super = visitor.getName();
}

std::vector<std::vector<std::string>> HierarchyVisitor::getInterfaces() {
  return std::move(interfaces);
}

std::vector<std::vector<std::string>> HierarchyVisitor::getExtensions() {
  return std::move(extensions);
}

std::vector<std::string> HierarchyVisitor::getSuper() {
  return std::move(super);
}

} // namespace Env
