#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("client detects extension error JoosW", "[client-filename]") {
  REQUIRE_FALSE(Client::verifyFileName(
      "Je_1_ClassDeclaration_WrongFileName_Dot.foo.java"));
  REQUIRE_FALSE(Client::verifyFileName("foo.txt"));
  REQUIRE(Client::verifyFileName("good.java"));
}
