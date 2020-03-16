#ifndef ENVHIERARCHY_HPP
#define ENVHIERARCHY_HPP

#include "EnvFileHeader.hpp"
#include <vector>
#include <string>

namespace Env {

class Hierarchy {
public:
  explicit Hierarchy(FileHeader &&header);
  const AST::Node *getASTNode() const;
  const Type &getType() const;
  const std::string &getIdentifier() const;

  const std::vector<std::string> &getPackage() const;
  void setPackage(std::vector<std::string> &&package);

private:
  FileHeader header;
  std::vector<std::string> package;
};

};

#endif // ENVHIERARCHY_HPP
