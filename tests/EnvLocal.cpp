#include "EnvLocal.hpp"
#include "EnvTypeBody.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("Local", "[local-variable]") {
  Env::Local tables;
  REQUIRE(tables.addVariable("outer", Env::TypeKeyword::Integer));
  REQUIRE(tables.addVariable("outer2", Env::TypeKeyword::Integer));
  tables.addVariableTable();
  REQUIRE(tables.addVariable("middle", Env::TypeKeyword::Integer));

  SECTION("Adding variable with existing name and existing variableType in "
          "middle block fails") {
    REQUIRE_FALSE(tables.addVariable("outer", Env::TypeKeyword::Integer));
    REQUIRE_FALSE(tables.addVariable("middle", Env::TypeKeyword::Integer));
  }

  SECTION("Adding variable with existing name and non-existing variableType in "
          "middle block fails") {
    REQUIRE_FALSE(tables.addVariable("outer", Env::TypeKeyword::Integer));
    REQUIRE_FALSE(tables.addVariable("middle", Env::TypeKeyword::Integer));
  }

  SECTION("Adding variable with non-existing name and existing variableType in "
          "middle block passes") {
    REQUIRE(tables.addVariable("different", Env::TypeKeyword::Integer));
    REQUIRE(tables.addVariable("different2", Env::TypeKeyword::Integer));
  }

  SECTION("Adding variable with non-existing name, non-existing variableType "
          "passes") {
    REQUIRE(tables.addVariable("different", Env::TypeKeyword::Integer));
    REQUIRE(tables.addVariable("different2", Env::TypeKeyword::Integer));
  }

  SECTION("Finding existing variable in middle block, while in middle block "
          "passes") {
    REQUIRE(tables.findVariable("middle"));
  }

  SECTION("Finding existing variable in parent block, while in middle block "
          "passes") {
    REQUIRE(tables.findVariable("outer"));
  }

  SECTION("Finding existing variable in middle block, while in parent block "
          "fails") {
    tables.removeVariableTable();
    REQUIRE_FALSE(tables.findVariable("middle"));
  }

  SECTION("Finding exisinting variable in parent block, while in parent block "
          "passes") {
    tables.removeVariableTable();
    REQUIRE(tables.findVariable("outer"));
  }

  SECTION("Add non-existing variable in a block passes and then adding it "
          "after the block passes") {
    tables.addVariableTable();
    REQUIRE(tables.addVariable("inner", Env::TypeKeyword::Integer));
    tables.removeVariableTable();
    REQUIRE(tables.addVariable("inner", Env::TypeKeyword::Integer));
  }

  SECTION("Add non-existing variable in 2 different block passes") {
    tables.addVariableTable();
    REQUIRE(tables.addVariable("inner", Env::TypeKeyword::Integer));
    tables.removeVariableTable();
    tables.addVariableTable();
    REQUIRE(tables.addVariable("inner", Env::TypeKeyword::Integer));
    tables.removeVariableTable();
  }

  SECTION("Offset calculation") {
    REQUIRE(tables.findVariable("outer")->offset == 16);
    REQUIRE(tables.findVariable("outer2")->offset == 12);
    REQUIRE(tables.findVariable("middle")->offset == -4);
    REQUIRE(tables.addVariable("inner", Env::TypeKeyword::Integer));
    REQUIRE(tables.findVariable("inner")->offset == -8);
  }
}
