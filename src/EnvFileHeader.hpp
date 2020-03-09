#ifndef ENVFILEHEADER_HPP
#define ENVFILEHEADER_HPP

#include <set>

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"

namespace Env {

enum class VariableType {
  SimpleType,
  ArrayType,
};

enum class Type {
  Class,
  Interface,
};

struct VariableDescriptor {
  const VariableType variableType;
  const std::string dataType;

  VariableDescriptor(VariableType variableType, std::string dataType);
  bool operator==(const VariableDescriptor &variableDescriptor) const;
};

struct TypeDescriptor {
  const Type type;
  const std::string identifier;

  TypeDescriptor(Type type, std::string identifier);
  bool operator==(const TypeDescriptor &typeDescriptor) const;
};

struct JoosField {
  const std::set<AST::ModifierCode> modifiers;
  const VariableDescriptor variableDescriptor;
  const std::string identifier;
  AST::Node *astNode;

  JoosField(std::set<AST::ModifierCode> modifiers, VariableDescriptor variableDescriptor,
            std::string identifier);
  bool operator==(const JoosField &joosField) const;
};

struct JoosMethod {
  const std::set<AST::ModifierCode> modifiers;
  const VariableDescriptor returnType;
  const std::string identifier;
  const std::vector<VariableDescriptor> args;
  AST::Node *astNode;
  // LocalVariableTable Root;

  JoosMethod(std::set<AST::ModifierCode> modifiers, VariableDescriptor returnType,
             std::string identifier, std::vector<VariableDescriptor> args);
  bool operator==(const JoosMethod &joosMethod) const;
};

struct FileHeader {
public:
  const TypeDescriptor typeDescriptor;
  const std::set<AST::ModifierCode> classModifiers;
  // std::unique_ptr<AST::Node> Node;

  FileHeader(std::set<AST::ModifierCode> classModifiers, TypeDescriptor typeDescriptor);
  bool addField(Env::JoosField joosField);
  bool addMethod(Env::JoosMethod joosMethod);
  const JoosField *findField(const VariableDescriptor &variableDescriptor,
                             const std::string &identifier) const;
  const JoosMethod *findMethod(const std::string &identifier,
                               const std::vector<VariableDescriptor> &args) const;
  const std::set<AST::ModifierCode> &getModifiers() const;
  const std::string &getName() const;
private:
  std::vector<JoosMethod> methods;
  std::vector<JoosField> fields;
};


class FileHeaderVisitor : public AST::TrackVisitor {
public:
  std::vector<JoosField> getJoosFields();
  std::vector<JoosMethod> getJoosMethods();
  void visit(const AST::FieldDeclaration &field) override;
  void visit(const AST::MethodDeclaration &decl) override;
  void visit(const AST::ConstructorDeclaration &decl) override;

private:
  std::vector<JoosField> fields;
  std::vector<JoosField> methods;
};

}; // namespace Env

#endif