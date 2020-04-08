#include "TypeChecker.hpp"
#include "catch.hpp"

TEST_CASE("Check type with function", "[TypeChecker]") {
  auto tree = std::make_shared<Env::PackageTree>();
  Type::Checker checker(*tree);

  SECTION("Transitivity of assignment") {
    auto type = checker.checkAssignment(Env::TypeKeyword::Integer,
                                        Env::TypeKeyword::Byte);
    REQUIRE(*type == Env::TypeKeyword::Integer);
  }

  SECTION("Self assignment") {
    REQUIRE(checker.checkAssignment(Env::TypeKeyword::Integer,
                                    Env::TypeKeyword::Integer));
  }

  SECTION("Null assignment to primitive") {
    REQUIRE_FALSE(checker.checkAssignment(Env::TypeKeyword::Integer,
                                          Env::TypeKeyword::Null));
  }

  SECTION("Null assignment to primitive") {
    REQUIRE(checker.checkAssignment(Env::TypeKeyword::Simple,
                                    Env::TypeKeyword::Null));
  }

  SECTION("Super class Assignment") {
    Env::TypeDeclaration listDecl({}, Env::DeclarationKeyword::Class, "List");
    Env::TypeDeclaration arrayDecl({}, Env::DeclarationKeyword::Class, "Array");
    listDecl.subType.emplace(&arrayDecl);
    REQUIRE_FALSE(
        checker.checkAssignment(Env::Type(&listDecl), Env::Type(&arrayDecl)));
    REQUIRE(
        checker.checkAssignment(Env::Type(&arrayDecl), Env::Type(&listDecl)));
  }

  SECTION("Num comparison") {
    Type::BinaryOperation operation{Type::BinaryOperator::Lesser,
                                    Env::TypeKeyword::Character,
                                    Env::TypeKeyword::Integer};
    REQUIRE(checker.checkBinaryOperation(operation));
  }

  SECTION("Subtype comparison") {
    Env::TypeDeclaration listDecl({}, Env::DeclarationKeyword::Class, "List");
    Env::TypeDeclaration arrayDecl({}, Env::DeclarationKeyword::Class, "Array");
    listDecl.subType.emplace(&arrayDecl);
    Type::BinaryOperation operation{Type::BinaryOperator::Equal,
                                    Env::Type{&listDecl},
                                    Env::Type{&arrayDecl}};
    REQUIRE(checker.checkBinaryOperation(operation));

    Type::BinaryOperation roperation{
        Type::BinaryOperator::Equal,
        Env::Type{&arrayDecl},
        Env::Type{&listDecl},
    };
    REQUIRE(checker.checkBinaryOperation(roperation));

    Type::BinaryOperation invalidOperation{
        Type::BinaryOperator::Lesser,
        Env::Type{&arrayDecl},
        Env::Type{&listDecl},
    };
    REQUIRE_FALSE(checker.checkBinaryOperation(invalidOperation));
  }

  SECTION("Boolean binaryOperation operation") {
    Type::BinaryOperation invalidOperation{Type::BinaryOperator::Or,
                                           Env::TypeKeyword::Integer,
                                           Env::TypeKeyword::Integer};
    REQUIRE_FALSE(checker.checkBinaryOperation(invalidOperation));
    Type::BinaryOperation validOperation{Type::BinaryOperator::Or,
                                         Env::TypeKeyword::Boolean,
                                         Env::TypeKeyword::Boolean};
    REQUIRE(checker.checkBinaryOperation(validOperation));
  }

  SECTION("UnaryOperation operation") {
    Type::UnaryOperation validBooleanOperation{
        Type::UnaryOperator::Not,
        Env::TypeKeyword::Boolean,
    };
    REQUIRE(checker.checkUnaryOperation(validBooleanOperation));
    Type::UnaryOperation validIntegerOperation{
        Type::UnaryOperator::Negative,
        Env::TypeKeyword::Integer,
    };
    REQUIRE(checker.checkUnaryOperation(validIntegerOperation));
    Type::UnaryOperation invalidOperation{
        Type::UnaryOperator::Negative,
        Env::TypeKeyword::Boolean,
    };
    REQUIRE_FALSE(checker.checkUnaryOperation(invalidOperation));
  }
}
