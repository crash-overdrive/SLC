#include "ASTVisitorUtil.hpp"
#include "EnvFileHeader.hpp"

namespace Env {

VariableDescriptor::VariableDescriptor(VariableType variableType,
                                       std::string dataType)
    : variableType(variableType), dataType(dataType){};

bool VariableDescriptor::
operator==(const VariableDescriptor &variableDescriptor) const {
  return variableType == variableDescriptor.variableType &&
         dataType == variableDescriptor.dataType;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
  return type == typeDescriptor.type && identifier == typeDescriptor.identifier;
}

JoosField::JoosField(std::set<AST::ModifierCode> modifiers,
                     VariableDescriptor variableDescriptor,
                     std::string identifier)
    : modifiers(modifiers), variableDescriptor(variableDescriptor),
      identifier(identifier){};

bool JoosField::operator==(const JoosField &joosField) const {
  return identifier == joosField.identifier &&
         variableDescriptor.dataType == joosField.variableDescriptor.dataType;
}

JoosMethod::JoosMethod(std::set<AST::ModifierCode> modifiers,
                       VariableDescriptor returnType, std::string identifier,
                       std::vector<VariableDescriptor> args)
    : modifiers(modifiers), returnType(returnType), identifier(identifier),
      args(args){};

bool JoosMethod::operator==(const JoosMethod &joosMethod) const {
  return identifier == joosMethod.identifier && args == joosMethod.args;
}

FileHeader::FileHeader(std::set<AST::ModifierCode> classModifiers,
                       TypeDescriptor typeDescriptor)
    : typeDescriptor(typeDescriptor), classModifiers(classModifiers){};

bool FileHeader::addField(JoosField joosField) {
  for (auto const &field : fields) {
    if (field == joosField) {
      return false;
    }
  }
  fields.emplace_back(std::move(joosField));
  return true;
}

bool FileHeader::addMethod(JoosMethod joosMethod) {
  for (auto const &method : methods) {
    if (method == joosMethod) {
      return false;
    }
  }
  methods.emplace_back(std::move(joosMethod));
  return true;
}

const JoosField *
FileHeader::findField(const VariableDescriptor &variableDescriptor,
                      const std::string &identifier) const {
  for (auto const &field : fields) {
    if (field.identifier == identifier &&
        field.variableDescriptor == variableDescriptor) {
      return &field;
    }
  }
  return nullptr;
}

const JoosMethod *
FileHeader::findMethod(const std::string &identifier,
                       const std::vector<VariableDescriptor> &args) const {
  for (auto const &method : methods) {
    if (method.identifier == identifier && method.args == args) {
      return &method;
    }
  }
  return nullptr;
}

const std::string &FileHeader::getName() const {
  return typeDescriptor.identifier;
}

const std::set<AST::ModifierCode> &FileHeader::getModifiers() const {
  return classModifiers;
}

void JoosTypeVisitor::visit(const AST::ClassDeclaration &decl) {
  typeDescriptor.type = Env::Type::Class;
  Node = &decl;
  visitProperties(decl);
}

void JoosTypeVisitor::visit(const AST::InterfaceDeclaration &decl) {
  typeDescriptor.type = Env::Type::Interface;
  Node = &decl;
  visitProperties(decl);
}

TypeDescriptor JoosTypeVisitor::getTypeDescriptor() {
  return typeDescriptor;
}

std::set<AST::ModifierCode> JoosTypeVisitor::getModifers() {
  return std::move(classModifiers);
}

const AST::Node *JoosTypeVisitor::getASTNode() {
  return Node;
}

void JoosTypeVisitor::visitProperties(const AST::Node &node) {
  AST::PropertiesVisitor Visitor;
  Visitor.dispatchChildren(node);
  typeDescriptor.identifier = Visitor.getName();
  classModifiers = Visitor.getModifiers();
}

} // namespace Env
