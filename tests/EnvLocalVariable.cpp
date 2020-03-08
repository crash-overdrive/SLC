#include "EnvLocalVariable.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("LocalVariableScope", "[local-scope]") {
  Env::LocalVariableScope Head;
  Env::LocalVariableScope *Node = &Head;

  SECTION("simple-test") {
    REQUIRE(Node->add("x"));
    REQUIRE(Node->lookUp("x"));
  }

  SECTION("simple-test") {
    REQUIRE_FALSE(Node->lookUp("y"));
  }

  SECTION("variable-shadowing") {
    REQUIRE(Node->add("x"));
    Node = Node->push();
    REQUIRE_FALSE(Node->add("x"));
  }

  SECTION("variable") {
    REQUIRE(Node->add("x"));
    Node = Node->push();
    Node = Node->pop();
    REQUIRE_FALSE(Node->add("x"));
  }

  SECTION("after-block") {
    REQUIRE(Node->add("x"));
    Node = Node->push();
    Node = Node->pop();
    REQUIRE(Node->lookUp("x"));
    REQUIRE_FALSE(Node->lookUp("y"));
  }
}
