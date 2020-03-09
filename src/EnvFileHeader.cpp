#include "EnvFileHeader.hpp"

Env::VariableDescriptor::VariableDescriptor(VariableType variableType, std::string dataType) :
  variableType(variableType), dataType(dataType) {};

bool Env::VariableDescriptor::operator==(const VariableDescriptor &variableDescriptor) const{
  return variableType == variableDescriptor.variableType && dataType == variableDescriptor.dataType;
}

Env::TypeDescriptor::TypeDescriptor(Type type, std::string identifier) :
  type(type), identifier(identifier) {};

bool Env::TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const{
  return type == typeDescriptor.type && identifier == typeDescriptor.identifier;
}

Env::JoosField::JoosField(std::set<AST::ModifierCode> modifiers, VariableDescriptor variableDescriptor, std::string identifier) :
  modifiers(modifiers), variableDescriptor(variableDescriptor), identifier(identifier) {};

bool Env::JoosField::operator==(const JoosField &joosField) const{
  return identifier == joosField.identifier && variableDescriptor.dataType == joosField.variableDescriptor.dataType;
}

Env::JoosMethod::JoosMethod(std::set<AST::ModifierCode> modifiers, VariableDescriptor returnType, std::string identifier, std::vector<VariableDescriptor> args) :
  modifiers(modifiers), returnType(returnType), identifier(identifier), args(args) {};

bool Env::JoosMethod::operator==(const JoosMethod &joosMethod) const{
  return identifier == joosMethod.identifier && args == joosMethod.args;
}

Env::FileHeader::FileHeader(std::set<AST::ModifierCode> classModifiers, TypeDescriptor typeDescriptor) :
  typeDescriptor(typeDescriptor), classModifiers(classModifiers) {};

bool Env::FileHeader::addField(Env::JoosField joosField) {
  for (auto const& field : fields) {
    if (field == joosField) {
      return false;
    }
  }
  fields.emplace_back(std::move(joosField));
  return true;
}

bool Env::FileHeader::addMethod(Env::JoosMethod joosMethod) {
  for (auto const& method : methods) {
    if (method == joosMethod) {
      return false;
    }
  }
  methods.emplace_back(std::move(joosMethod));
  return true;
}

const Env::JoosField *Env::FileHeader::findField(const Env::VariableDescriptor &variableDescriptor, const std::string &identifier) const {
  for (auto const& field : fields) {
    if (field.identifier == identifier && field.variableDescriptor == variableDescriptor) {
      return &field;
    }
  }
  return nullptr;
}

const Env::JoosMethod *Env::FileHeader::findMethod(const std::string &identifier, const std::vector<VariableDescriptor> &args) const{
  for (auto const& method : methods) {
    if (method.identifier == identifier && method.args == args) {
      return &method;
    }
  }
  return nullptr;
}
