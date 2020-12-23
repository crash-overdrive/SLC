#ifndef TYPECHECKER_HPP
#define TYPECHECKER_HPP

#include "EnvPackageTree.hpp"
#include <array>
#include <unordered_set>
#include <vector>

namespace Type {

const std::unordered_set<Env::TypeKeyword> numKeyword{
    Env::TypeKeyword::Integer,
    Env::TypeKeyword::Short,
    Env::TypeKeyword::Character,
    Env::TypeKeyword::Byte,
};

const std::vector<std::array<Env::TypeKeyword, 2>> primitiveAssignment{
    {Env::TypeKeyword::Simple, Env::TypeKeyword::Null},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Short},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Byte},
    {Env::TypeKeyword::Integer, Env::TypeKeyword::Character},
    {Env::TypeKeyword::Short, Env::TypeKeyword::Byte},
};

enum class BinaryOperator {
  Addition,
  Subtraction,
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
  BitwiseOr,
  BitwiseAnd,
  BitwiseXor,
};

enum class UnaryOperator {
  Negative,
  Not,
};

const std::unordered_map<std::string, BinaryOperator> symbolBinaryOperator{
    {"+", BinaryOperator::Addition},
    {"-", BinaryOperator::Subtraction},
    {"*", BinaryOperator::Multiplication},
    {"/", BinaryOperator::Division},
    {"%", BinaryOperator::Modulus},
    {"==", BinaryOperator::Equal},
    {"!=", BinaryOperator::NotEqual},
    {"<=", BinaryOperator::LesserOrEqual},
    {">=", BinaryOperator::GreaterOrEqual},
    {"<", BinaryOperator::Lesser},
    {">", BinaryOperator::Greater},
    {"||", BinaryOperator::Or},
    {"&&", BinaryOperator::And},
    {"|", BinaryOperator::BitwiseOr},
    {"&", BinaryOperator::BitwiseAnd},
    {"^", BinaryOperator::BitwiseXor},
};

const std::unordered_map<std::string, UnaryOperator> symbolUnaryOperator{
    {"-", UnaryOperator::Negative},
    {"!", UnaryOperator::Not},
};

const std::unordered_set<Type::BinaryOperator> numBinaryOperator{
    Type::BinaryOperator::Addition,       Type::BinaryOperator::Subtraction,
    Type::BinaryOperator::Multiplication, Type::BinaryOperator::Division,
    Type::BinaryOperator::Modulus,
};

const std::unordered_set<Type::BinaryOperator> boolBinaryOperator{
    Type::BinaryOperator::Equal,      Type::BinaryOperator::NotEqual,
    Type::BinaryOperator::Or,         Type::BinaryOperator::And,
    Type::BinaryOperator::BitwiseOr,  Type::BinaryOperator::BitwiseAnd,
    Type::BinaryOperator::BitwiseXor,
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
  std::optional<Env::Type> checkArrayAccess(Env::Type lopt,
                                            Env::Type ropt) const;
  std::optional<Env::Type> checkArrayCreation(Env::Type lopt,
                                              Env::Type ropt) const;

private:
  bool isStringAddition(Env::Type lopt, Env::Type ropt) const;
  bool isNum(Env::Type type) const;
  bool isAssignable(Env::Type lopt, Env::Type ropt) const;
  const Env::PackageTree &tree;
};

} // namespace Type

#endif // TYPECHECKER_HPP
