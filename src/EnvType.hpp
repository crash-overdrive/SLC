#ifndef ENVTYPE_HPP
#define ENVTYPE_HPP

#include <iostream>
#include <unordered_map>

namespace Env {

struct TypeDeclaration;

enum class TypeKeyword {
  Integer,
  Boolean,
  Byte,
  Short,
  Null,
  Void,
  Simple,
};

const std::unordered_map<std::string, TypeKeyword> stringTypeKeyword{
    {"INT", TypeKeyword::Integer},   {"BOOLEAN", TypeKeyword::Boolean},
    {"BYTE", TypeKeyword::Byte},     {"SHORT", TypeKeyword::Short},
    {"NULL", TypeKeyword::Null},     {"VOID", TypeKeyword::Void},
    {"SIMPLE", TypeKeyword::Simple},
};

const std::unordered_map<TypeKeyword, std::string> typeKeywordString{
    {TypeKeyword::Integer, "INT"},   {TypeKeyword::Boolean, "BOOLEAN"},
    {TypeKeyword::Byte, "BYTE"},     {TypeKeyword::Short, "SHORT"},
    {TypeKeyword::Null, "NULL"},     {TypeKeyword::Void, "VOID"},
    {TypeKeyword::Simple, "SIMPLE"},
};

struct Type {
  TypeKeyword keyword;
  bool isArray = false;
  TypeDeclaration *declare = nullptr;
  bool operator==(const Type &type) const;
  bool operator!=(const Type &type) const;
};
std::ostream &operator<<(std::ostream &stream, const Type &type);

} // namespace Env

#endif // ENVTYPE_HPP
