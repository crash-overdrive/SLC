#include "EnvHierarchy.hpp"
#include "EnvTypeLink.hpp"

namespace Env {

Hierarchy::Hierarchy(FileHeader header) : header(std::move(header)) {}

const AST::Node *Hierarchy::getASTNode() const { return header.getASTNode(); }

bool Hierarchy::findModifiers(AST::ModifierCode code) const {
  return header.findModifiers(code);
}

const std::string &Hierarchy::getIdentifier() const {
  return header.getIdentifier();
}

Type Hierarchy::getType() const { return header.getType(); }

const std::vector<std::string> &Hierarchy::getPackage() const {
  return package;
}

void Hierarchy::setPackage(std::vector<std::string> package) {
  this->package = std::move(package);
}

const TypeLink *Hierarchy::getTypeLink() const { return typeLink.get(); }

void Hierarchy::setTypeLink(std::unique_ptr<TypeLink> typeLink) {
  this->typeLink = std::move(typeLink);
}

InterfaceHierarchy::InterfaceHierarchy(FileHeader header)
    : Hierarchy(std::move(header)) {}

bool InterfaceHierarchy::addExtends(const Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Interface) {
    return false;
  }
  auto [it, flag] = extends.emplace(hierarchy);
  return flag;
}

bool InterfaceHierarchy::subType(const Hierarchy *hierarchy) const {
  (void)hierarchy;
  return false;
}

ClassHierarchy::ClassHierarchy(FileHeader header)
    : Hierarchy(std::move(header)) {}

bool ClassHierarchy::setExtends(const Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Class ||
      hierarchy->findModifiers(AST::ModifierCode::Final)) {
    return false;
  }
  extends = hierarchy;
  return true;
}

bool ClassHierarchy::addImplements(const Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Interface) {
    return false;
  }
  auto [it, flag] = implements.emplace(hierarchy);
  return flag;
}

bool ClassHierarchy::subType(const Hierarchy *hierarchy) const {
  (void)hierarchy;
  return false;
}

InterfaceHierarchy &HierarchyGraph::addInterface(FileHeader header) {
  auto ptr = std::make_unique<Env::InterfaceHierarchy>(std::move(header));
  auto &hierarchy = interfaces.emplace_back(std::move(ptr));
  hierarchies.emplace_back(hierarchy.get());
  return *hierarchy;
}

ClassHierarchy &HierarchyGraph::addClass(FileHeader header) {
  auto ptr = std::make_unique<Env::ClassHierarchy>(std::move(header));
  auto &hierarchy = classes.emplace_back(std::move(ptr));
  hierarchies.emplace_back(hierarchy.get());
  return *hierarchy;
}

std::vector<std::unique_ptr<InterfaceHierarchy>> &
HierarchyGraph::getInterfaces() {
  return interfaces;
}

std::vector<std::unique_ptr<ClassHierarchy>> &HierarchyGraph::getClasses() {
  return classes;
}

std::vector<Hierarchy *> &HierarchyGraph::getHierarchies() {
  return hierarchies;
}

bool HierarchyGraph::topologicalSort() {
  std::list<DAGNode> nodes = augmentGraph();
  std::set<DAGNode *> workList;
  std::list<Hierarchy *> order;
  for (auto &node : nodes) {
    if (node.inDegree == 0) {
      workList.emplace(&node);
    }
  }
  while (workList.size()) {
    auto it = workList.begin();
    DAGNode *node = *it;
    order.emplace_front(node->hierarchy);
    for (auto &outNode : node->outNodes) {
      --outNode->inDegree;
      if (!outNode->inDegree) {
        workList.emplace(outNode);
      }
    }
    workList.erase(it);
  }
  if (order.size() != hierarchies.size()) {
    return false;
  }
  hierarchies = std::vector<Hierarchy *>(order.begin(), order.end());
  return true;
}

bool HierarchyGraph::buildSubType() { return true; }

HierarchyGraph::DAGNode::DAGNode(Hierarchy *hierarchy) : hierarchy(hierarchy) {}

void HierarchyGraph::DAGNode::addOutNode(DAGNode *node) {
  outNodes.emplace_back(node);
  ++node->inDegree;
}

std::list<HierarchyGraph::DAGNode> HierarchyGraph::augmentGraph() {
  std::list<DAGNode> nodes;
  std::unordered_map<const Hierarchy *, DAGNode *> dagMap;
  for (const auto &hierarchy : hierarchies) {
    DAGNode &node = nodes.emplace_back(hierarchy);
    dagMap.emplace(hierarchy, &node);
  }
  for (const auto &interfaceType : interfaces) {
    DAGNode *node = dagMap.at(interfaceType.get());
    for (const auto &extend : interfaceType->extends) {
      node->addOutNode(dagMap.at(extend));
    }
  }
  for (const auto &classType : classes) {
    DAGNode *node = dagMap.at(classType.get());
    if (classType->extends) {
      node->addOutNode(dagMap.at(classType->extends));
    }
    for (const auto &implement : classType->implements) {
      node->addOutNode(dagMap.at(implement));
    }
  }
  return nodes;
}

} // namespace Env
