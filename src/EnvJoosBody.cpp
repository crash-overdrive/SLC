#include "EnvJoosBody.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

VariableDescriptor::VariableDescriptor(VariableType variableType,
                                       std::vector<std::string> dataType)
    : variableType(std::move(variableType)), dataType(std::move(dataType)) {}

bool VariableDescriptor::
operator==(const VariableDescriptor &variableDescriptor) const {
  return variableType == variableDescriptor.variableType &&
         dataType == variableDescriptor.dataType;
}

std::ostream &operator<<(std::ostream &stream,
                         const VariableDescriptor &variableDescriptor) {
  stream << "VARIABLE DESCRIPTOR: "
         << "{";
  stream << "VariableType: "
         << Env::variableTypeName.at(variableDescriptor.variableType) << "; ";
  stream << "DataType: ";
  for (auto const &dataType : variableDescriptor.dataType) {
    stream << dataType << " ";
  }
  stream << "}\n";
  return stream;
}

JoosField::JoosField(std::set<Modifier> modifiers,
                     VariableDescriptor descriptor, std::string identifier,
                     const AST::Node *astNode)
    : modifiers(std::move(modifiers)),
      variableDescriptor(std::move(descriptor)),
      identifier(std::move(identifier)), astNode(astNode) {}

bool JoosField::operator==(const JoosField &joosField) const {
  return identifier == joosField.identifier;
}

std::ostream &operator<<(std::ostream &stream, const JoosField &joosField) {
  stream << "JOOSFIELD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosField.modifiers) {
    stream << modifierName.at(modifier) << " ";
  }
  stream << "}\n";
  stream << joosField.variableDescriptor;
  stream << "Identifier: " << joosField.identifier << "\n";
  return stream << "\n";
}

JoosMethod::JoosMethod(std::set<Modifier> modifiers,
                       VariableDescriptor returnType, std::string identifier,
                       std::vector<VariableDescriptor> args,
                       const AST::Node *astNode)
    : modifiers(std::move(modifiers)), returnType(std::move(returnType)),
      identifier(std::move(identifier)), args(std::move(args)),
      astNode(astNode) {}

bool JoosMethod::operator==(const JoosMethod &joosMethod) const {
  return identifier == joosMethod.identifier && args == joosMethod.args;
}

std::ostream &operator<<(std::ostream &stream, const JoosMethod &joosMethod) {
  stream << "JOOSMETHOD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosMethod.modifiers) {
    stream << modifierName.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "ReturnType: ";
  stream << joosMethod.returnType;
  stream << "Identifier: " << joosMethod.identifier << "\n";
  for (auto const &arg : joosMethod.args) {
    stream << "Argument: " << arg;
  }
  return stream << "\n";
}

JoosConstructor::JoosConstructor(std::set<Modifier> modifiers,
                                 std::string identifier,
                                 std::vector<VariableDescriptor> args,
                                 const AST::Node *astNode)
    : modifiers(std::move(modifiers)), identifier(std::move(identifier)),
      args(std::move(args)), astNode(astNode) {}

bool JoosConstructor::operator==(const JoosConstructor &joosConstructor) const {
  return identifier == joosConstructor.identifier &&
         args == joosConstructor.args;
}

std::ostream &operator<<(std::ostream &stream,
                         const JoosConstructor &joosConstructor) {
  stream << "JOOSCONSTRUCTOR"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosConstructor.modifiers) {
    stream << modifierName.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "Identifier: " << joosConstructor.identifier << "\n";
  for (auto const &arg : joosConstructor.args) {
    stream << "Argument: " << arg;
  }
  return stream << "\n";
}

const std::vector<JoosField> &JoosBody::getFields() const { return fields; }

bool JoosBody::addField(JoosField joosField) {
  for (auto const &field : fields) {
    if (field == joosField) {
      return false;
    }
  }
  fields.emplace_back(std::move(joosField));
  return true;
}

const std::vector<JoosMethod> &JoosBody::getMethods() const { return methods; }

bool JoosBody::addMethod(JoosMethod joosMethod) {
  for (auto const &method : methods) {
    if (method == joosMethod) {
      return false;
    }
  }
  methods.emplace_back(std::move(joosMethod));
  return true;
}

const std::vector<JoosConstructor> &JoosBody::getConstructors() const {
  return constructors;
}

bool JoosBody::addConstructor(JoosConstructor joosConstructor) {
  for (auto const &constructor : constructors) {
    if (constructor == joosConstructor) {
      return false;
    }
  }
  constructors.emplace_back(std::move(joosConstructor));
  return true;
}

const JoosField *
JoosBody::findField(const VariableDescriptor &variableDescriptor,
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
JoosBody::findMethod(const std::string &identifier,
                     const std::vector<VariableDescriptor> &args) const {
  for (auto const &method : methods) {
    if (method.identifier == identifier && method.args == args) {
      return &method;
    }
  }
  return nullptr;
}

const JoosConstructor *
JoosBody::findConstructor(const std::string &identifier,
                          const std::vector<VariableDescriptor> &args) const {
  for (auto const &constructor : constructors) {
    if (constructor.identifier == identifier && constructor.args == args) {
      return &constructor;
    }
  }
  return nullptr;
}

std::ostream &operator<<(std::ostream &stream, const JoosBody &joosBody) {
  for (auto const &field : joosBody.fields) {
    stream << field;
  }
  for (auto const &method : joosBody.methods) {
    stream << method;
  }
  for (auto const &constructor : joosBody.constructors) {
    stream << constructor;
  }
  return stream << '\n';
}

void JoosBodyVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void JoosBodyVisitor::visit(const AST::ClassDeclaration &decl) {
  dispatchChildren(decl);
}

void JoosBodyVisitor::visit(const AST::InterfaceDeclaration &decl) {
  dispatchChildren(decl);
}

void JoosBodyVisitor::visit(const AST::FieldDeclaration &field) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(field);
  fields.emplace_back(propertiesVisitor.getModifiers(),
                      propertiesVisitor.getVariableDescriptor(),
                      propertiesVisitor.getIdentifier(), &field);
}

void JoosBodyVisitor::visit(const AST::MethodDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  methods.emplace_back(propertiesVisitor.getModifiers(),
                       propertiesVisitor.getVariableDescriptor(),
                       propertiesVisitor.getIdentifier(),
                       argumentsVisitor.getArgs(), &decl);
}

void JoosBodyVisitor::visit(const AST::ConstructorDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  constructors.emplace_back(propertiesVisitor.getModifiers(),
                            propertiesVisitor.getIdentifier(),
                            argumentsVisitor.getArgs(), &decl);
}

std::vector<JoosField> JoosBodyVisitor::getJoosFields() {
  return std::move(fields);
}

std::vector<JoosMethod> JoosBodyVisitor::getJoosMethods() {
  return std::move(methods);
}

std::vector<JoosConstructor> JoosBodyVisitor::getJoosConstructors() {
  return std::move(constructors);
}

} // namespace Env
