#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "EnvFileHeader.hpp"
#include <string>
#include <vector>

namespace Env {

class Hierarchy {
public:
  explicit Hierarchy(FileHeader &&header);
  const AST::Node *getASTNode() const;
  const Type &getType() const;
  const std::string &getIdentifier() const;

  const std::vector<std::string> &getPackage() const;
  void setPackage(std::vector<std::string> &&package);
  void setExtension(Hierarchy *hierarchy);
  void addInterface(Hierarchy *hierarchy);

private:
  FileHeader header;
  std::vector<std::string> package;
  Hierarchy *extension = nullptr;
  std::vector<Hierarchy *> interfaces;
};

} // namespace Env

#endif // ENVHIERARCHY_HPP
