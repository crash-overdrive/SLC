#ifndef ENVPACKAGETREE_HPP
#define ENVPACKAGETREE_HPP

#include "ASTVisitor.hpp"
#include "EnvFileHeader.hpp"
#include <map>
#include <string>

namespace Env {

class PackageNode {
public:
  enum Type { Global, Package, JoosType };

  PackageNode(Type type, const std::string &name = "", FileHeader *header
              = nullptr);
  PackageNode *update(Type type, const std::string &name,
                      FileHeader *Header = nullptr);
  PackageNode *find(const std::string &name);

private:
  friend class PackageTree;
  PackageNode *updatePackage(Type type, const std::string &name);
  PackageNode *addType(Type type, const std::string &name,
                       FileHeader *header = nullptr);
  Type type;
  std::string name;
  FileHeader *header;
  std::map<std::string, PackageNode> children{};
};

class PackageTree {
public:
  FileHeader *lookUp(const std::vector<std::string> &PackagePath);
  bool update(const std::vector<std::string> &PackagePath, FileHeader &Header);

private:
  std::unique_ptr<PackageNode> Root =
      std::make_unique<PackageNode>(PackageNode::Global);
};

class PackageTreeVisitor : public AST::TrackVisitor {
public:
  void visit(const AST::PackageDeclaration &Decl) override;
  // Avoid unecessary traversal
  void visit(const AST::ClassDeclaration &Decl) override;
  void visit(const AST::InterfaceDeclaration &Decl) override;
  std::vector<std::string> getPackagePath() const;

private:
  std::vector<std::string> packagePath;
};

} // namespace Env

#endif // ENVSCOPE_HPP
