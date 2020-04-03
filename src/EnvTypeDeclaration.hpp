#ifndef ENVTYPEDECLARATION_HPP
#define ENVTYPEDECLARATION_HPP

#include "EnvTypeBody.hpp"
#include "EnvTypeContain.hpp"
#include <unordered_set>

namespace Env {

enum class DeclarationKeyword {
  Class,
  Interface,
};

const std::unordered_map<DeclarationKeyword, std::string> declareKeywordString{
    {DeclarationKeyword::Class, "Class"},
    {DeclarationKeyword::Interface, "Interface"},
};

struct TypeDeclaration {
  std::set<Modifier> modifiers;
  DeclarationKeyword keyword;
  std::string identifier;
  std::unique_ptr<AST::Node> astNode;
  TypeBody body;
  std::unordered_set<const TypeDeclaration *> subType;
  TypeContain contain;

  TypeDeclaration(std::set<Modifier> modifiers, DeclarationKeyword keyword,
                  std::string identifier,
                  std::unique_ptr<AST::Node> astNode = nullptr);
};
std::ostream &operator<<(std::ostream &stream, const TypeDeclaration &decl);

class TypeDeclarationVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  std::set<Modifier> getModifiers();
  std::string getIdentifier();
  DeclarationKeyword getKeyword();

private:
  void visitProperties(const AST::Node &node);
  std::set<Modifier> modifiers;
  DeclarationKeyword keyword;
  std::string identifier;
};

} // namespace Env

#endif // ENVTYPEDECLARATION_HPP
