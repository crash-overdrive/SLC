#include <set>

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

namespace Env {

struct JoosField {
  std::set<AST::ModifierCode> Modifiers;
  std::string Type;
  std::string Name;

  JoosField(std::set<AST::ModifierCode> Modifiers, std::string Type,
            std::string Name);
  AST::Node *Node;
  bool operator==(const JoosField &Field);
};

struct JoosMethod {
  std::set<AST::ModifierCode> Modifiers;
  std::string ReturnType;
  std::string Name;
  std::vector<std::string> ArgType;

  JoosMethod(std::set<AST::ModifierCode> Modifiers, std::string ReturnType,
             std::string Name, std::vector<std::string> ArgType);
  AST::Node *Node;
  // LocalVariableTable Root;
  bool operator==(const JoosMethod &Method);
};

struct FileHeader {
public:
  bool addField(Env::JoosField &&joosField);
  bool addMethod(Env::JoosMethod &&joosMethod);
  const JoosField *findField(const std::string &Type,
                             const std::string &Name) const;
  const JoosMethod *findMethod(const std::string &Name,
                               const std::vector<std::string> &ArgType) const;

private:
  std::vector<JoosMethod> Methods;
  std::vector<JoosField> Fields;
};


class FileHeaderVisitor : public AST::TrackVisitor {
public:
  std::vector<JoosField> getJoosFields();
  std::vector<JoosMethod> getJoosMethods();
  void visit(const AST::FieldDeclaration &Field) override;
  void visit(const AST::MethodDeclaration &Decl) override;
  void visit(const AST::ConstructorDeclaration &Decl) override;

private:
  std::vector<JoosField> Fields;
  std::vector<JoosField> Methods;
};

}; // namespace Env

