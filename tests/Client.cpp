#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("client detects extension error JoosW", "[client-filename]") {
  REQUIRE_FALSE(Client::verifyFileName(
      "Je_1_ClassDeclaration_WrongFileName_Dot.foo.java"));
  REQUIRE_FALSE(Client::verifyFileName("foo.txt"));
  REQUIRE(Client::verifyFileName("good.java"));
}

TEST_CASE("client scanner detects JoosW", "[client-scanner][!hide]") {
  SECTION("scanner accept") {
    std::ostringstream OStream;
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir + "/java/J1_publicclasses.java");
    REQUIRE(Client::scan(JoosStream, OStream));

    std::ifstream TokenStream;
    TokenStream.open(TestDataDir + "/grammar/J1_publicclasses.tokens");
    std::string TokenString(std::istreambuf_iterator<char>(TokenStream), {});
    REQUIRE(TokenString == OStream.str());
  }

  SECTION("scanner rejects") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/java/a1/Je_1_NonJoosConstructs_Unicode.java");
    REQUIRE_FALSE(Client::scan(JoosStream));
  }
}


TEST_CASE("client preprocessor detects JoosW", "[client-preprocess][!hide]") {
  SECTION("preprocessor parser reject") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/java/a1/Je_1_NonJoosConstructs_Volatile.java");
    REQUIRE_FALSE(Client::preProcess(JoosStream,
                                "Je_1_NonJoosConstructs_Volatile.java"));
  }
}
