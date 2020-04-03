#ifndef TYPECHECKER_HPP
#define TYPECHECKER_HPP

#include "EnvTypeLink.hpp"
#include <unordered_set>
#include <vector>

namespace Type {

const std::unordered_set<Env::TypeKeyword> numKeyword{
    Env::TypeKeyword::Integer,
    Env::TypeKeyword::Short,
    Env::TypeKeyword::Character,
    Env::TypeKeyword::Byte,
};

enum class BinaryOperator {
  Addition,
  Substraction,
  Multiplication,
  Division,
  Modulus,
  Equal,
  LesserOrEqual,
  GreaterOrEqual,
  Lesser,
  Greater,
  NotEqual,
  Or,
  And,
};

struct BinaryOperation {
  BinaryOperator binaryOperator;
  Env::Type lopt;
  Env::Type ropt;
};

struct UnaryOperation {
  BinaryOperator binaryOperator;
  Env::Type opt;
};

std::optional<Env::Type> checkBinaryOperation(const BinaryOperation &operation);
std::optional<Env::Type> checkUnaryOperation(const UnaryOperation &operation);
std::optional<Env::Type> checkCasting(Env::Type lopt, Env::Type ropt);
std::optional<Env::Type> checkInstanceOf(Env::Type lopt, Env::Type ropt);
std::optional<Env::Type> checkArrayIndex(Env::Type lopt, Env::Type ropt);
std::optional<Env::Type> checkAssignment(Env::Type lopt, Env::Type ropt);
bool isNum(const Env::Type &type);

} // namespace Type

#endif // TYPECHECKER_HPP
