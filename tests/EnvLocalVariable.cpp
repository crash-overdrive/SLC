#include "EnvLocalVariable.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("LocalVariableScope", "[local-scope]") {
  Env::LocalVariableScope head;
  Env::LocalVariableScope *node = &head;

  SECTION("simple-test") {
    REQUIRE(node->add("x"));
    REQUIRE(node->lookUp("x"));
  }

  SECTION("simple-test") {
    REQUIRE_FALSE(node->lookUp("y"));
  }

  SECTION("variable-shadowing") {
    REQUIRE(node->add("x"));
    node = node->push();
    REQUIRE_FALSE(node->add("x"));
  }

  SECTION("variable") {
    REQUIRE(node->add("x"));
    node = node->push();
    node = node->pop();
    REQUIRE_FALSE(node->add("x"));
  }

  SECTION("after-block") {
    REQUIRE(node->add("x"));
    node = node->push();
    node = node->pop();
    REQUIRE(node->lookUp("x"));
    REQUIRE_FALSE(node->lookUp("y"));
  }
}
