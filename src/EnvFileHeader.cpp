#include "EnvFileHeader.hpp"

Env::JoosField::JoosField(std::set<AST::ModifierCode> Modifiers, std::string Type, std::string Name) :
  Modifiers(Modifiers), Type(Type), Name(Name) {};

bool Env::JoosField::operator==(const Env::JoosField &Field) {
  return Modifiers == Field.Modifiers && Type == Field.Type && Name == Field.Name;
}

Env::JoosMethod::JoosMethod(std::set<AST::ModifierCode> Modifiers, std::string ReturnType, std::string Name, std::vector<std::string> ArgType) :
  Modifiers(Modifiers), ReturnType(ReturnType), Name(Name), ArgType(ArgType) {};

bool Env::JoosMethod::operator==(const Env::JoosMethod &Method) {
  return Modifiers == Method.Modifiers && ReturnType == Method.ReturnType && Name == Method.Name && ArgType == Method.ArgType;
}

Env::FileHeader::FileHeader(std::string Name, std::set<AST::ModifierCode> Modifiers, Env::Type type) :
  Name(Name), Modifiers(Modifiers), type(type) {};

bool Env::FileHeader::addField(Env::JoosField &&joosField) {
  for (auto const& Field : Fields) {
    if (Field.Name == joosField.Name && Field.Type == joosField.Type) {
      return false;
    }
  }
  Fields.emplace_back(std::move(joosField));
  return true;
}

bool Env::FileHeader::addMethod(Env::JoosMethod &&joosMethod) {
  for (auto const& Method : Methods) {
    if (Method.Name == joosMethod.Name && Method.ArgType == joosMethod.ArgType) {
      return false;
    }
  }
  Methods.emplace_back(std::move(joosMethod));
  return true;
}

const Env::JoosField *Env::FileHeader::findField(const std::string &Type, const std::string &Name) const {
  for (auto const& Field : Fields) {
    if (Field.Name == Name && Field.Type == Type) {
      return &Field;
    }
  }
  return nullptr;
}

const Env::JoosMethod *Env::FileHeader::findMethod(const std::string &Name, const std::vector<std::string> &ArgType) const{
  for (auto const& Method : Methods) {
    if (Method.Name == Name && Method.ArgType == ArgType) {
      return &Method;
    }
  }
  return nullptr;
}
