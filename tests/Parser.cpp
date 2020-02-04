#include "Parser.hpp"
#include "catch.hpp"

TEST_CASE("Done return true", "[parser]") {
  Parser Parser;

  REQUIRE(!Parser.done());
}
