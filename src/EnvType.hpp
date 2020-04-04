#ifndef ENVTYPE_HPP
#define ENVTYPE_HPP

#include <iostream>
#include <unordered_map>

namespace Env {

struct TypeDeclaration;

enum class TypeKeyword {
  None,
  Integer,
  Boolean,
  Byte,
  Short,
  Null,
  Void,
  Character,
  Simple,
};

const std::unordered_map<std::string, TypeKeyword> stringTypeKeyword{
    {"INT", TypeKeyword::Integer},    {"BOOLEAN", TypeKeyword::Boolean},
    {"BYTE", TypeKeyword::Byte},      {"SHORT", TypeKeyword::Short},
    {"NULL", TypeKeyword::Null},      {"VOID", TypeKeyword::Void},
    {"CHAR", TypeKeyword::Character}, {"SIMPLE", TypeKeyword::Simple},
};

const std::unordered_map<TypeKeyword, std::string> typeKeywordString{
    {TypeKeyword::Integer, "INT"},    {TypeKeyword::Boolean, "BOOLEAN"},
    {TypeKeyword::Byte, "BYTE"},      {TypeKeyword::Short, "SHORT"},
    {TypeKeyword::Null, "NULL"},      {TypeKeyword::Void, "VOID"},
    {TypeKeyword::Character, "CHAR"}, {TypeKeyword::Simple, "SIMPLE"},
};

struct Type {
  TypeKeyword keyword;
  const TypeDeclaration *declare;
  bool isArray;
  Type(TypeKeyword keyword = TypeKeyword::None, bool isArray = false);
  explicit Type(const TypeDeclaration *declare, bool isArray = false);
  bool operator==(const Type &type) const;
  bool operator!=(const Type &type) const;
};
std::ostream &operator<<(std::ostream &stream, const Type &type);

} // namespace Env

#endif // ENVTYPE_HPP
