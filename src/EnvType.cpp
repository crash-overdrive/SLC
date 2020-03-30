#include "EnvType.hpp"

namespace Env {

Type::Type(TypeKeyword keyword, bool isArray, TypeDeclaration *declare)
    : keyword(keyword), isArray(isArray), declare(declare) {}

bool Type::operator==(const Type &type) const {
  return keyword == type.keyword && isArray == type.isArray &&
         declare == type.declare;
}

bool Type::operator!=(const Type &type) const {
  return keyword != type.keyword || isArray != type.isArray ||
         declare != type.declare;
}

std::ostream &operator<<(std::ostream &stream, const Type &type) {
  stream << typeKeywordString.at(type.keyword) << '\n';
  return stream;
}

} // namespace Env
