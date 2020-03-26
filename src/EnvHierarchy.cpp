#include "EnvHierarchy.hpp"
#include "ASTVisitorUtil.hpp"
#include "EnvTypeLink.hpp"
#include <algorithm>

namespace Env {

InterfaceHierarchy::InterfaceHierarchy(JoosType &joosType)
    : joosType(joosType) {}

bool InterfaceHierarchy::addExtends(const JoosType *joosType) {
  if (joosType->type != Type::Interface) {
    return false;
  }
  auto [it, flag] = extends.emplace(joosType);
  return flag;
}

bool InterfaceHierarchy::setBaseObject(const JoosType *base) {
  if (extends.size()) {
    return true;
  }
  for (const auto &method : base->declare.getMethods()) {
    if (!joosType.declare.addMethod(method)) {
      return false;
    }
  }
  return true;
}

void InterfaceHierarchy::buildSubType() const {
  for (const auto &extend : extends) {
    joosType.subType.insert(extend->subType.begin(), extend->subType.end());
  }
  joosType.subType.emplace(&joosType);
}

bool InterfaceHierarchy::buildContains() const {
  joosType.declare.setAbstract();
  for (const auto &extend : extends) {
    if (!joosType.contain.mergeContain(extend->contain)) {
      return false;
    }
  }
  for (const auto &method : joosType.declare.getMethods()) {
    if (!joosType.contain.addDeclareMethod(&method)) {
      return false;
    }
  }
  return true;
}

ClassHierarchy::ClassHierarchy(JoosType &joosType) : joosType(joosType) {}

bool ClassHierarchy::setExtends(const JoosType *joosType) {
  if (joosType->type != Type::Class ||
      joosType->modifiers.find(Modifier::Final) != joosType->modifiers.end()) {
    return false;
  }
  extends = joosType;
  return true;
}

bool ClassHierarchy::addImplements(const JoosType *joosType) {
  if (joosType->type != Type::Interface) {
    return false;
  }
  auto [it, flag] = implements.emplace(joosType);
  return flag;
}

bool ClassHierarchy::setBaseObject(const JoosType *base) {
  if (!extends && base->astNode != joosType.astNode) {
    extends = base;
  }
  return true;
}

void ClassHierarchy::buildSubType() const {
  if (extends) {
    joosType.subType.insert(extends->subType.begin(), extends->subType.end());
  }
  for (const auto &implement : implements) {
    joosType.subType.insert(implement->subType.begin(),
                            implement->subType.end());
  }
  joosType.subType.emplace(&joosType);
}

bool ClassHierarchy::buildContains() const {
  if (extends && !joosType.contain.mergeContain(extends->contain)) {
    return false;
  }
  for (const auto &implement : implements) {
    if (!joosType.contain.mergeContain(implement->contain)) {
      return false;
    }
  }
  for (const auto &field : joosType.declare.getFields()) {
    joosType.contain.addDeclareField(&field);
  }
  for (const auto &method : joosType.declare.getMethods()) {
    if (!joosType.contain.addDeclareMethod(&method)) {
      std::cerr << method;
      return false;
    }
  }
  return !(joosType.contain.hasAbstract() &&
           joosType.modifiers.find(Modifier::Abstract) ==
               joosType.modifiers.end());
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
  std::unordered_map<const JoosType *, DAGNode *> dagMap;
  for (auto &interfaceType : interfaces) {
    DAGNode &node = nodes.emplace_back(interfaceType);
    dagMap.emplace(&interfaceType.joosType, &node);
  }
  for (auto &classType : classes) {
    DAGNode &node = nodes.emplace_back(classType);
    dagMap.emplace(&classType.joosType, &node);
  }

  for (const auto &interfaceType : interfaces) {
    DAGNode *node = dagMap.at(&interfaceType.joosType);
    for (const auto &extend : interfaceType.extends) {
      node->addOutNode(dagMap.at(extend));
    }
  }
  for (const auto &classType : classes) {
    DAGNode *node = dagMap.at(&classType.joosType);
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
