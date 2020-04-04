#ifndef TYPECHECKER_HPP
#define TYPECHECKER_HPP

#include "EnvPackageTree.hpp"
#include <unordered_set>
#include <vector>

namespace Type {

const std::unordered_set<Env::TypeKeyword> numKeyword{
    Env::TypeKeyword::Integer,
    Env::TypeKeyword::Short,
    Env::TypeKeyword::Character,
    Env::TypeKeyword::Byte,
};

const std::vector<std::array<Env::Type, 2>> primitiveAssignment{
    {Env::TypeKeyword::Simple, Env::TypeKeyword::Null},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Short},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Byte},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Character},
    {Env::TypeKeyword::Short, Env::TypeKeyword::Byte},
};

enum class BinaryOperator {
  Addition,
  Substraction,
  Multiplication,
  Division,
  Modulus,
  Equal,
  NotEqual,
  LesserOrEqual,
  GreaterOrEqual,
  Lesser,
  Greater,
  Or,
  And,
};

const std::unordered_set<Type::BinaryOperator> numBinaryOperator{
    Type::BinaryOperator::Addition,       Type::BinaryOperator::Substraction,
    Type::BinaryOperator::Multiplication, Type::BinaryOperator::Division,
    Type::BinaryOperator::Modulus,
};

const std::unordered_set<Type::BinaryOperator> boolBinaryOperator{
    Type::BinaryOperator::Equal,
    Type::BinaryOperator::NotEqual,
    Type::BinaryOperator::Or,
    Type::BinaryOperator::And,
};

const std::unordered_set<Type::BinaryOperator> compBinaryOperator{
    Type::BinaryOperator::Equal,
    Type::BinaryOperator::NotEqual,
};

const std::unordered_set<Type::BinaryOperator> latticeBinaryOperator{
    Type::BinaryOperator::Equal,          Type::BinaryOperator::LesserOrEqual,
    Type::BinaryOperator::GreaterOrEqual, Type::BinaryOperator::Lesser,
    Type::BinaryOperator::Greater,        Type::BinaryOperator::NotEqual,
};

struct BinaryOperation {
  BinaryOperator binaryOperator;
  Env::Type lopt;
  Env::Type ropt;
};

enum class UnaryOperator {
  Negative,
  Not,
};

struct UnaryOperation {
  UnaryOperator unaryOperator;
  Env::Type opt;
};

class Checker {
public:
  Checker(const Env::PackageTree &tree);
  std::optional<Env::Type> checkAssignment(Env::Type lopt,
                                           Env::Type ropt) const;
  std::optional<Env::Type>
  checkBinaryOperation(const BinaryOperation &operation) const;
  std::optional<Env::Type>
  checkUnaryOperation(const UnaryOperation &operation) const;
  std::optional<Env::Type> checkCasting(Env::Type lopt, Env::Type ropt) const;
  std::optional<Env::Type> checkInstanceOf(Env::Type lopt,
                                           Env::Type ropt) const;
  std::optional<Env::Type> checkArrayIndex(Env::Type lopt,
                                           Env::Type ropt) const;

private:
  std::optional<Env::Type> checkArrayAssignment(Env::Type lopt,
                                                Env::Type ropt) const;
  bool isStringAddition(Env::Type lopt, Env::Type ropt) const;
  bool isNum(Env::Type type) const;
  bool isAssignable(Env::Type lopt, Env::Type ropt) const;
  const Env::PackageTree &tree;
};

} // namespace Type

#endif // TYPECHECKER_HPP
