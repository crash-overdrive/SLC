#include "EnvHierarchy.hpp"
#include "EnvTypeLink.hpp"

namespace Env {

Hierarchy::Hierarchy(FileHeader header) : header(std::move(header)) {}

const AST::Node *Hierarchy::getASTNode() const { return header.getASTNode(); }

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

bool InterfaceHierarchy::addExtends(Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Interface) {
    return false;
  }
  auto [it, flag] = implements.emplace(hierarchy);
  return flag;
}

ClassHierarchy::ClassHierarchy(FileHeader header)
    : Hierarchy(std::move(header)) {}

bool ClassHierarchy::setExtends(Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Class) {
    return false;
  }
  extends = hierarchy;
  return true;
}

bool ClassHierarchy::addImplements(Hierarchy *hierarchy) {
  if (hierarchy->getType() != Type::Interface) {
    return false;
  }
  auto [it, flag] = implements.emplace(hierarchy);
  return flag;
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

bool HierarchyGraph::topologicalSort() const { return false; }

} // namespace Env
