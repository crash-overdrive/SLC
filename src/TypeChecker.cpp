#include "TypeChecker.hpp"

namespace Type {

Checker::Checker(const Env::PackageTree &tree) : tree(tree) {}

std::optional<Env::Type> Checker::checkAssignment(Env::Type lopt,
                                                  Env::Type ropt) const {
  if (lopt == ropt) {
    return lopt;
  }
  if (ropt.isArray &&
      (lopt.declare == tree.findDeclaration({"java", "lang", "Object"}) ||
       lopt.declare == tree.findDeclaration({"java", "lang", "Cloneable"}) ||
       lopt.declare == tree.findDeclaration({"java", "io", "Serializable"}))) {
    return lopt;
  }
  if (lopt.isArray && ropt.keyword == Env::TypeKeyword::Null) {
    return lopt;
  }
  if (lopt.isArray != ropt.isArray) {
    return std::nullopt;
  }

  if (lopt.keyword == Env::TypeKeyword::Simple &&
      ropt.keyword == Env::TypeKeyword::Simple &&
      ropt.declare->subType.find(lopt.declare) != ropt.declare->subType.end()) {
    return lopt;
  }
  for (const auto &assignment : primitiveAssignment) {
    if (lopt.keyword == assignment.at(0) && ropt.keyword == assignment.at(1)) {
      return lopt;
    }
  }
  return std::nullopt;
}

std::optional<Env::Type>
Checker::checkBinaryOperation(const BinaryOperation &operation) const {
  Env::TypeDeclaration *string =
      tree.findDeclaration({"java", "lang", "String"});
  if ((operation.lopt.declare == string || operation.ropt.declare == string) &&
      operation.binaryOperator == BinaryOperator::Addition) {
    return Env::Type(string);
  }

  if (isNum(operation.lopt) && isNum(operation.ropt)) {
    if (numBinaryOperator.find(operation.binaryOperator) !=
        numBinaryOperator.end()) {
      return Env::TypeKeyword::Integer;
    } else if (latticeBinaryOperator.find(operation.binaryOperator) !=
               latticeBinaryOperator.end()) {
      return Env::TypeKeyword::Boolean;
    } else {
      return std::nullopt;
    }
  }
  if (operation.lopt.keyword == Env::TypeKeyword::Boolean &&
      operation.ropt.keyword == Env::TypeKeyword::Boolean) {
    if (boolBinaryOperator.find(operation.binaryOperator) !=
        boolBinaryOperator.end()) {
      return Env::TypeKeyword::Boolean;
    } else {
      return std::nullopt;
    }
  }
  if (compBinaryOperator.find(operation.binaryOperator) !=
      compBinaryOperator.end()) {
    if (isAssignable(operation.lopt, operation.ropt)) {
      return Env::TypeKeyword::Boolean;
    } else {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

std::optional<Env::Type>
Checker::checkUnaryOperation(const UnaryOperation &operation) const {
  switch (operation.unaryOperator) {
  case UnaryOperator::Negative:
    if (isNum(operation.opt)) {
      return Env::TypeKeyword::Integer;
    }
    break;
  case UnaryOperator::Not:
    if (operation.opt.keyword == Env::TypeKeyword::Boolean) {
      return Env::TypeKeyword::Boolean;
    }
    break;
  }
  return std::nullopt;
}

std::optional<Env::Type> Checker::checkCasting(Env::Type lopt,
                                               Env::Type ropt) const {
  if (ropt.keyword == Env::TypeKeyword::Null || (isNum(lopt) && isNum(ropt)) ||
      isAssignable(lopt, ropt)) {
    return lopt;
  }
  return std::nullopt;
}

std::optional<Env::Type> Checker::checkInstanceOf(Env::Type lopt,
                                                  Env::Type ropt) const {
  if (isAssignable(lopt, ropt)) {
    return Env::TypeKeyword::Boolean;
  }
  return std::nullopt;
}

std::optional<Env::Type> Checker::checkArrayAccess(Env::Type lopt,
                                                   Env::Type ropt) const {
  if (lopt.isArray && isNum(ropt)) {
    lopt.isArray = false;
    return lopt;
  }
  return std::nullopt;
}

std::optional<Env::Type> Checker::checkArrayAssignment(Env::Type lopt,
                                                       Env::Type ropt) const {
  if (lopt.keyword != Env::TypeKeyword::Simple ||
      ropt.keyword != Env::TypeKeyword::Simple) {
    return std::nullopt;
  }
  if (lopt.isArray &&
      lopt.declare->subType.find(ropt.declare) != lopt.declare->subType.end()) {
    return lopt;
  }
  return std::nullopt;
}

std::optional<Env::Type> Checker::checkArrayCreation(Env::Type lopt,
                                                     Env::Type ropt) const {
  if (!isNum(ropt)) {
    return std::nullopt;
  }
  lopt.isArray = true;
  return lopt;
}

bool Checker::isNum(Env::Type type) const {
  return numKeyword.find(type.keyword) != numKeyword.end();
}

bool Checker::isAssignable(Env::Type lopt, Env::Type ropt) const {
  return checkAssignment(lopt, ropt) || checkAssignment(ropt, lopt);
}

} // namespace Type
