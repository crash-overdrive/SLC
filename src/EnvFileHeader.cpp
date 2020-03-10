#include "ASTVisitorUtil.hpp"
#include "EnvFileHeader.hpp"

namespace Env {

// VariableDescriptor::VariableDescriptor(VariableType variableType,
//                                        std::string dataType)
//     : variableType(variableType), dataType(dataType){};

bool VariableDescriptor::
operator==(const VariableDescriptor &variableDescriptor) const {
  return variableType == variableDescriptor.variableType &&
         dataType == variableDescriptor.dataType;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
  return type == typeDescriptor.type && identifier == typeDescriptor.identifier;
}

// JoosField::JoosField(std::set<AST::ModifierCode> modifiers,
//                      VariableDescriptor variableDescriptor,
//                      std::string identifier)
//     : modifiers(modifiers), variableDescriptor(variableDescriptor),
//       identifier(identifier){};

bool JoosField::operator==(const JoosField &joosField) const {
  return identifier == joosField.identifier &&
         variableDescriptor.dataType == joosField.variableDescriptor.dataType;
}

// JoosMethod::JoosMethod(std::set<AST::ModifierCode> modifiers,
//                        VariableDescriptor returnType, std::string identifier,
//                        std::vector<VariableDescriptor> args)
//     : modifiers(modifiers), returnType(returnType), identifier(identifier),
//       args(args){};

bool JoosMethod::operator==(const JoosMethod &joosMethod) const {
  return identifier == joosMethod.identifier && args == joosMethod.args;
}

// JoosConstructor::JoosConstructor(std::set<AST::ModifierCode> modifiers,
//                        std::string identifier,
//                        std::vector<VariableDescriptor> args)
//     : modifiers(modifiers), identifier(identifier),
//       args(args){};

bool JoosConstructor::operator==(const JoosConstructor &joosConstructor) const {
  return identifier == joosConstructor.identifier && args == joosConstructor.args;
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

bool FileHeader::addConstructor(JoosConstructor joosConstructor) {
  for (auto const &constructor : constructors) {
    if (constructor == joosConstructor) {
      return false;
    }
  }
  constructors.emplace_back(std::move(joosConstructor));
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

const JoosConstructor *
FileHeader::findConstructor(const std::string &identifier,
                       const std::vector<VariableDescriptor> &args) const {
  for (auto const &constructor : constructors) {
    if (constructor.identifier == identifier && constructor.args == args) {
      return &constructor;
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

void JoosTypeVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
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
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  typeDescriptor.identifier = propertiesVisitor.getIdentifier();
  classModifiers = propertiesVisitor.getModifiers();
}

void JoosTypeBodyVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void JoosTypeBodyVisitor::visit(const AST::ClassDeclaration &decl) {
  dispatchChildren(decl);
}

void JoosTypeBodyVisitor::visit(const AST::InterfaceDeclaration &decl) {
  dispatchChildren(decl);
}

void JoosTypeBodyVisitor::visit(const AST::FieldDeclaration &field) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(field);
  fields.push_back(Env::JoosField{propertiesVisitor.getModifiers(),
                      propertiesVisitor.getVariableDescriptor(),
                      propertiesVisitor.getIdentifier(),
                      &field});
}

void JoosTypeBodyVisitor::visit(const AST::MethodDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  methods.push_back(Env::JoosMethod{propertiesVisitor.getModifiers(),
                      propertiesVisitor.getVariableDescriptor(),
                      propertiesVisitor.getIdentifier(),
                      argumentsVisitor.getArgs(),
                      &decl});
}

void JoosTypeBodyVisitor::visit(const AST::ConstructorDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  constructors.push_back(Env::JoosConstructor{propertiesVisitor.getModifiers(),
                      propertiesVisitor.getIdentifier(),
                      argumentsVisitor.getArgs(),
                      &decl});
}

std::vector<JoosField> JoosTypeBodyVisitor::getJoosFields() {
  return std::move(fields);
}

std::vector<JoosMethod> JoosTypeBodyVisitor::getJoosMethods() {
  return std::move(methods);
}

std::vector<JoosConstructor> JoosTypeBodyVisitor::getJoosConstructors() {
  return std::move(constructors);
}

std::ostream &operator<<(std::ostream &stream, const JoosField &joosField) {
  stream << "JoosField: " << "\n";
  stream << "Modifiers: ";
  for (auto const &modifier : joosField.modifiers) {
    stream << AST::ModifierCodeName.at(modifier) << ", ";
  }
  stream << "\n" << "VariableType: " << Env::VariableTypeName.at(joosField.variableDescriptor.variableType) << "\n";
  stream << "DataType: ";
  for (auto const &dataType : joosField.variableDescriptor.dataType) {
    stream << dataType << ", ";
  }
  stream << "\n" << "Identifier: " << joosField.identifier << "\n";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const JoosMethod &joosMethod) {
  stream << "JoosMethod" << "\n";
  stream << "Modifiers: ";
  for (auto const &modifier : joosMethod.modifiers) {
    stream << AST::ModifierCodeName.at(modifier) << ", ";
  }
  stream << "\n" << "Return VariableType: " << Env::VariableTypeName.at(joosMethod.returnType.variableType) << "\n";
  stream << "Return DataType: ";
  for (auto const &dataType : joosMethod.returnType.dataType) {
    stream << dataType << ", ";
  }
  stream << "\n" << "Identifier: " << joosMethod.identifier << "\n";
  stream << "Arguments: ";
  for (auto const &arg : joosMethod.args) {
    stream << "\n" << "Argument VariableType: " << Env::VariableTypeName.at(arg.variableType) << "\n";
    stream << "Argument DataType: ";
    for (auto const &dataType : arg.dataType) {
      stream << dataType << ", ";
    }
  }
  stream << "\n";
  return stream;
}

} // namespace Env
