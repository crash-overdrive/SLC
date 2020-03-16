#include "EnvHierarchy.hpp"

namespace Env {

Hierarchy::Hierarchy(FileHeader &&header) : header(std::move(header)) {}

const std::vector<std::string> &Hierarchy::getPackage() const {
  return package;
}

void Hierarchy::setPackage(std::vector<std::string> &&package) {
  this->package = std::move(package);
}

const AST::Node *Hierarchy::getASTNode() const {
  return header.getASTNode();
}

const Type &Hierarchy::getType() const {
  return header.getType();
}

const std::string &Hierarchy::getIdentifier() const {
  return header.getIdentifier();
}

} // namespace Env
