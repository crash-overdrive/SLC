#include <set>

#include "ASTNode.hpp"

namespace Env {
struct JoosField {
  std::set<AST::ModifierCode> Modifiers;
  std::string Type;
  std::string Name;

  JoosField(std::set<AST::ModifierCode> Modifiers, std::string Type, std::string Name);
  AST::Node *Node;
  bool operator==(const JoosField &Field);
};

struct JoosMethod {
  std::set<AST::ModifierCode> Modifiers;
  std::string ReturnType;
  std::string Name;
  std::vector<std::string> ArgType;

  JoosMethod(std::set<AST::ModifierCode> Modifiers, std::string ReturnType, std::string Name, std::vector<std::string> ArgType);
  AST::Node *Node;
  // LocalVariableTable Root;
  bool operator==(const JoosMethod &Method);
};

struct FileHeader {
public:
  // std::unique_ptr<AST::Node> Node;
  bool addField(Env::JoosField &&joosField);
  bool addMethod(Env::JoosMethod &&joosMethod);
  const JoosField *findField(const std::string &Type, const std::string &Name) const;
  const JoosMethod *findMethod(const std::string &Name, const std::vector<std::string> &ArgType) const;
private:
  std::vector<JoosMethod> Methods;
  std::vector<JoosField> Fields;
};
}