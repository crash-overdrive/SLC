#include "EnvJoosType.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

bool VariableDescriptor::
operator==(const VariableDescriptor &variableDescriptor) const {
  return variableType == variableDescriptor.variableType &&
         dataType == variableDescriptor.dataType;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
  return type == typeDescriptor.type && identifier == typeDescriptor.identifier;
}

bool JoosField::operator==(const JoosField &joosField) const {
  return identifier == joosField.identifier;
}

bool JoosMethod::operator==(const JoosMethod &joosMethod) const {
  return identifier == joosMethod.identifier && args == joosMethod.args;
}

bool JoosConstructor::operator==(const JoosConstructor &joosConstructor) const {
  return identifier == joosConstructor.identifier &&
         args == joosConstructor.args;
}

FileHeader::FileHeader(std::set<AST::ModifierCode> modifiers,
                       TypeDescriptor typeDescriptor,
                       std::unique_ptr<AST::Node> node)
    : node(std::move(node)), modifiers(std::move(modifiers)),
      typeDescriptor(std::move(typeDescriptor)){};

const AST::Node *FileHeader::getASTNode() const { return node.get(); }

bool FileHeader::findModifiers(AST::ModifierCode code) const {
  return modifiers.find(code) != modifiers.end();
}

const std::string &FileHeader::getIdentifier() const {
  return typeDescriptor.identifier;
}

const Type &FileHeader::getType() const { return typeDescriptor.type; }

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

void JoosTypeVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void JoosTypeVisitor::visit(const AST::ClassDeclaration &decl) {
  typeDescriptor.type = Env::Type::Class;
  node = &decl;
  visitProperties(decl);
}

void JoosTypeVisitor::visit(const AST::InterfaceDeclaration &decl) {
  typeDescriptor.type = Env::Type::Interface;
  node = &decl;
  visitProperties(decl);
}

TypeDescriptor JoosTypeVisitor::getTypeDescriptor() { return typeDescriptor; }

std::set<AST::ModifierCode> JoosTypeVisitor::getModifiers() {
  return std::move(modifiers);
}

const AST::Node *JoosTypeVisitor::getASTNode() { return node; }

void JoosTypeVisitor::visitProperties(const AST::Node &node) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  typeDescriptor.identifier = propertiesVisitor.getIdentifier();
  modifiers = propertiesVisitor.getModifiers();
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
                                  propertiesVisitor.getIdentifier(), &field});
}

void JoosTypeBodyVisitor::visit(const AST::MethodDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  methods.push_back({propertiesVisitor.getModifiers(),
                     propertiesVisitor.getVariableDescriptor(),
                     propertiesVisitor.getIdentifier(),
                     argumentsVisitor.getArgs(), &decl});
}

void JoosTypeBodyVisitor::visit(const AST::ConstructorDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  AST::ArgumentsVisitor argumentsVisitor;

  propertiesVisitor.dispatchChildren(decl);
  argumentsVisitor.dispatchChildren(decl);
  constructors.push_back({propertiesVisitor.getModifiers(),
                          propertiesVisitor.getIdentifier(),
                          argumentsVisitor.getArgs(), &decl});
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

std::ostream &operator<<(std::ostream &stream,
                         const TypeDescriptor &typeDescriptor) {
  stream << "TYPE DESCRIPTOR: "
         << "{";
  stream << "Type: " << Env::typeName.at(typeDescriptor.type) << "; ";
  stream << "Identifier: " << typeDescriptor.identifier << "}\n";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const JoosField &joosField) {
  stream << "JOOSFIELD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosField.modifiers) {
    stream << AST::modifierCodeName.at(modifier) << " ";
  }
  stream << "}\n";
  stream << joosField.variableDescriptor;
  stream << "Identifier: " << joosField.identifier << "\n";
  return stream << "\n";
}

std::ostream &operator<<(std::ostream &stream, const JoosMethod &joosMethod) {
  stream << "JOOSMETHOD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosMethod.modifiers) {
    stream << AST::modifierCodeName.at(modifier) << " ";
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

std::ostream &operator<<(std::ostream &stream,
                         const JoosConstructor &joosConstructor) {
  stream << "JOOSCONSTRUCTOR"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : joosConstructor.modifiers) {
    stream << AST::modifierCodeName.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "Identifier: " << joosConstructor.identifier << "\n";
  for (auto const &arg : joosConstructor.args) {
    stream << "Argument: " << arg;
  }
  return stream << "\n";
}

std::ostream &operator<<(std::ostream &stream, const FileHeader &fileHeader) {
  stream << "TYPE MODIFIERS: {";
  for (auto const &modifier : fileHeader.modifiers) {
    stream << AST::modifierCodeName.at(modifier) << " ";
  }
  stream << "}\n\n";
  stream << fileHeader.typeDescriptor << '\n';
  for (auto const &field : fileHeader.fields) {
    stream << field;
  }
  for (auto const &method : fileHeader.methods) {
    stream << method;
  }
  for (auto const &constructor : fileHeader.constructors) {
    stream << constructor;
  }
  return stream << '\n';
}

} // namespace Env
