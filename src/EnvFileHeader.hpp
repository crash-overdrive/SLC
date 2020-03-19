#ifndef ENVFILEHEADER_HPP
#define ENVFILEHEADER_HPP

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"
#include <set>

namespace Env {

enum class VariableType {
  SimpleType,
  ArrayType,
};

enum class Type {
  Class,
  Interface,
};

const std::unordered_map<VariableType, std::string> variableTypeName{
    {VariableType::SimpleType, "SimpleType"},
    {VariableType::ArrayType, "ArrayType"},
};

const std::unordered_map<Type, std::string> typeName{
    {Type::Class, "Class"},
    {Type::Interface, "Interface"},
};

struct VariableDescriptor {
  VariableType variableType;
  std::vector<std::string> dataType;
  bool operator==(const VariableDescriptor &variableDescriptor) const;
};
std::ostream &operator<<(std::ostream &stream,
                         const VariableDescriptor &variableDescriptor);

struct TypeDescriptor {
  Type type;
  std::string identifier;
  bool operator==(const TypeDescriptor &typeDescriptor) const;
};
std::ostream &operator<<(std::ostream &stream,
                         const TypeDescriptor &typeDescriptor);

struct JoosField {
  std::set<AST::ModifierCode> modifiers;
  VariableDescriptor variableDescriptor;
  std::string identifier;
  const AST::Node *astNode;
  bool operator==(const JoosField &joosField) const;
};
std::ostream &operator<<(std::ostream &stream, const JoosField &joosField);

struct JoosMethod {
  std::set<AST::ModifierCode> modifiers;
  VariableDescriptor returnType;
  std::string identifier;
  std::vector<VariableDescriptor> args;
  const AST::Node *astNode;
  bool operator==(const JoosMethod &joosMethod) const;
};
std::ostream &operator<<(std::ostream &stream, const JoosMethod &joosMethod);

struct JoosConstructor {
  std::set<AST::ModifierCode> modifiers;
  std::string identifier;
  std::vector<VariableDescriptor> args;
  const AST::Node *astNode;
  bool operator==(const JoosConstructor &joosConstructor) const;
};
std::ostream &operator<<(std::ostream &stream,
                         const JoosConstructor &joosConstructor);

struct FileHeader {
public:
  FileHeader(std::set<AST::ModifierCode> modifiers,
             TypeDescriptor typeDescriptor,
             std::unique_ptr<AST::Node> node = nullptr);
  const AST::Node *getASTNode() const;
  bool findModifiers(AST::ModifierCode code) const;
  const std::string &getIdentifier() const;
  const Type &getType() const;

  bool addField(JoosField joosField);
  bool addMethod(JoosMethod joosMethod);
  bool addConstructor(JoosConstructor joosConstructor);
  const JoosField *findField(const VariableDescriptor &variableDescriptor,
                             const std::string &identifier) const;
  const JoosMethod *
  findMethod(const std::string &identifier,
             const std::vector<VariableDescriptor> &args) const;
  const JoosConstructor *
  findConstructor(const std::string &identifier,
                  const std::vector<VariableDescriptor> &args) const;
  void setPackage(std::vector<std::string> package);

private:
  friend std::ostream &operator<<(std::ostream &stream,
                                  const FileHeader &fileHeader);
  std::unique_ptr<AST::Node> node;
  std::set<AST::ModifierCode> modifiers;
  TypeDescriptor typeDescriptor;
  std::vector<JoosMethod> methods;
  std::vector<JoosField> fields;
  std::vector<JoosConstructor> constructors;
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
  void visitProperties(const AST::Node &node);
  std::set<AST::ModifierCode> modifiers;
  TypeDescriptor typeDescriptor;
  const AST::Node *node;
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
