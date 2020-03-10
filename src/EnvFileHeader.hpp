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

const std::unordered_map<VariableType, std::string> VariableTypeName {
  {VariableType::SimpleType, "SimpleType"},
  {VariableType::ArrayType, "ArrayType"},
};

const std::unordered_map<Type, std::string> TypeName {
  {Type::Class, "Class"},
  {Type::Interface, "Interface"},
};

struct VariableDescriptor {
  VariableType variableType;
  std::vector<std::string> dataType;

  // VariableDescriptor(VariableType variableType, std::string dataType);
  bool operator==(const VariableDescriptor &variableDescriptor) const;
};
std::ostream &operator<<(std::ostream &stream, const VariableDescriptor &variableDescriptor);

struct TypeDescriptor {
  Type type;
  std::string identifier;

  bool operator==(const TypeDescriptor &typeDescriptor) const;
};
std::ostream &operator<<(std::ostream &stream, const TypeDescriptor &typeDescriptor);

struct JoosField {
  std::set<AST::ModifierCode> modifiers;
  VariableDescriptor variableDescriptor;
  std::string identifier;
  const AST::Node *astNode;

  // JoosField(std::set<AST::ModifierCode> modifiers, VariableDescriptor variableDescriptor,
  //           std::string identifier);
  bool operator==(const JoosField &joosField) const;
};
std::ostream &operator<<(std::ostream &stream, const JoosField &joosField);

struct JoosMethod {
  std::set<AST::ModifierCode> modifiers;
  VariableDescriptor returnType;
  std::string identifier;
  std::vector<VariableDescriptor> args;
  const AST::Node *astNode;
  // LocalVariableTable Root;

  // JoosMethod(std::set<AST::ModifierCode> modifiers, VariableDescriptor returnType,
  //            std::string identifier, std::vector<VariableDescriptor> args);
  bool operator==(const JoosMethod &joosMethod) const;
};
std::ostream &operator<<(std::ostream &stream, const JoosMethod &joosMethod);

struct JoosConstructor {
  std::set<AST::ModifierCode> modifiers;
  std::string identifier;
  std::vector<VariableDescriptor> args;
  const AST::Node *astNode;
  // LocalVariableTable Root;

  // JoosConstructor(std::set<AST::ModifierCode> modifiers,
  //            std::string identifier, std::vector<VariableDescriptor> args);
  bool operator==(const JoosConstructor &joosConstructor) const;
};
std::ostream &operator<<(std::ostream &stream, const JoosConstructor &joosConstructor);

struct FileHeader {
public:
  TypeDescriptor typeDescriptor;
  std::set<AST::ModifierCode> classModifiers;
  std::unique_ptr<AST::Node> Node;

  FileHeader(std::set<AST::ModifierCode> classModifiers, TypeDescriptor typeDescriptor);
  bool addField(JoosField joosField);
  bool addMethod(JoosMethod joosMethod);
  bool addConstructor(JoosConstructor joosConstructor);
  const JoosField *findField(const VariableDescriptor &variableDescriptor,
                             const std::string &identifier) const;
  const JoosMethod *findMethod(const std::string &identifier,
                               const std::vector<VariableDescriptor> &args) const;
  const JoosConstructor *findConstructor(const std::string &identifier,
                               const std::vector<VariableDescriptor> &args) const;
  const std::set<AST::ModifierCode> &getModifiers() const;
  const std::string &getName() const;

private:
  std::vector<JoosField> fields;
  std::vector<JoosMethod> methods;
  std::vector<JoosConstructor> constructors;
  friend std::ostream &operator<<(std::ostream &stream, const FileHeader &fileHeader);
};
std::ostream &operator<<(std::ostream &stream, const FileHeader &fileHeader);

class JoosTypeVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  TypeDescriptor getTypeDescriptor();
  std::set<AST::ModifierCode> getModifiers();
  const AST::Node *getASTNode();

private:
  std::set<AST::ModifierCode> classModifiers;
  TypeDescriptor typeDescriptor;
  const AST::Node *Node;

  void visitProperties(const AST::Node &node);
};

class JoosTypeBodyVisitor : public AST::Visitor {
public:
  void visit(const AST::Start &start) override;
  void visit(const AST::ClassDeclaration &decl) override;
  void visit(const AST::InterfaceDeclaration &decl) override;
  void visit(const AST::FieldDeclaration &field) override;
  void visit(const AST::MethodDeclaration &decl) override;
  void visit(const AST::ConstructorDeclaration &decl) override;
  std::vector<JoosField> getJoosFields();
  std::vector<JoosMethod> getJoosMethods();
  std::vector<JoosConstructor> getJoosConstructors();

private:
  std::vector<JoosField> fields;
  std::vector<JoosMethod> methods;
  std::vector<JoosConstructor> constructors;
};

}; // namespace Env

#endif
