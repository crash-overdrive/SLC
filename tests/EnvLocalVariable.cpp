#include "TestConfig.hpp"
#include "catch.hpp"

#include "EnvJoosBody.hpp"
#include "EnvLocalVariable.hpp"

TEST_CASE("LocalVariableAnalysis", "[local-variable]") {
  Env::LocalVariableAnalysis localVariableAnalysis(false);
  REQUIRE(localVariableAnalysis.addVariable(
      "outer",
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));
  localVariableAnalysis.addEnvironment();
  REQUIRE(localVariableAnalysis.addVariable(
      "middle",
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));

  SECTION("Adding variable with existing name and existing variableType in "
          "middle block fails") {
    REQUIRE_FALSE(localVariableAnalysis.addVariable(
        "outer",
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));
    REQUIRE_FALSE(localVariableAnalysis.addVariable(
        "middle",
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));
  }

  SECTION("Adding variable with existing name and non-existing variableType in "
          "middle block fails") {
    REQUIRE_FALSE(localVariableAnalysis.addVariable(
        "outer",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
    REQUIRE_FALSE(localVariableAnalysis.addVariable(
        "middle",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
  }

  SECTION("Adding variable with non-existing name and existing variableType in "
          "middle block passes") {
    REQUIRE(localVariableAnalysis.addVariable(
        "different",
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));
    REQUIRE(localVariableAnalysis.addVariable(
        "different2",
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}));
  }

  SECTION("Adding variable with non-existing name, non-existing variableType "
          "passes") {
    REQUIRE(localVariableAnalysis.addVariable(
        "different",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
    REQUIRE(localVariableAnalysis.addVariable(
        "different2",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
  }

  SECTION("Finding existing variable in middle block, while in middle block "
          "passes") {
    REQUIRE(localVariableAnalysis.findVariable("middle"));
  }

  SECTION("Finding existing variable in parent block, while in middle block "
          "passes") {
    REQUIRE(localVariableAnalysis.findVariable("outer"));
  }

  SECTION("Finding existing variable in middle block, while in parent block "
          "fails") {
    localVariableAnalysis.removeEnvironment();
    REQUIRE_FALSE(localVariableAnalysis.findVariable("middle"));
  }

  SECTION("Finding exisinting variable in parent block, while in parent block "
          "passes") {
    localVariableAnalysis.removeEnvironment();
    REQUIRE(localVariableAnalysis.findVariable("outer"));
  }

  SECTION("Add non-existing variable in a block passes and then adding it "
          "after the block passes") {
    localVariableAnalysis.addEnvironment();
    REQUIRE(localVariableAnalysis.addVariable(
        "inner",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
    localVariableAnalysis.removeEnvironment();
    REQUIRE(localVariableAnalysis.addVariable(
        "inner",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
  }

  SECTION("Add non-existing variable in 2 different block passes") {
    localVariableAnalysis.addEnvironment();
    REQUIRE(localVariableAnalysis.addVariable(
        "inner",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
    localVariableAnalysis.removeEnvironment();
    localVariableAnalysis.addEnvironment();
    REQUIRE(localVariableAnalysis.addVariable(
        "inner",
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}}));
    localVariableAnalysis.removeEnvironment();
  }
}
