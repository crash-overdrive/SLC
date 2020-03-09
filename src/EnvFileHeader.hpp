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

enum class Type {
  Class,
  Interface,
};

struct FileHeader {
public:
  // std::unique_ptr<AST::Node> Node;
  FileHeader(std::string Name, std::set<AST::ModifierCode> Modifiers, Type type);
  bool addField(Env::JoosField &&joosField);
  bool addMethod(Env::JoosMethod &&joosMethod);
  const JoosField *findField(const std::string &Type, const std::string &Name) const;
  const JoosMethod *findMethod(const std::string &Name, const std::vector<std::string> &ArgType) const;
private:
  const std::string Name;
  const std::set<AST::ModifierCode> Modifiers;
  const Type type;
  std::vector<JoosMethod> Methods;
  std::vector<JoosField> Fields;
};
}