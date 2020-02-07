#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("client detects JoosW", "[client]") {
  Parse::DFA Parser;
  std::ostringstream OStream;
  std::ifstream LRStream;
  Client Client;
  LRStream.open(JoosLRFile);

  Parser.configure(LRStream);
  Client.setParser(Parser);
  Client.setOutput(OStream);

  SECTION("scanner only accept") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir + "/marmoset/J1_publicclasses.java");
    REQUIRE(Client.scanOnly(JoosStream));

    std::ifstream TokenStream;
    TokenStream.open(TestDataDir + "/grammar/J1_publicclasses.token");
    std::string TokenString(std::istreambuf_iterator<char>(TokenStream), {});
    REQUIRE(TokenString == OStream.str());
  }

  SECTION("parser only accept") {
    std::ifstream TokenStream;
    TokenStream.open(TestDataDir + "/grammar/J1_publicclasses.token");
    REQUIRE(Client.parseOnly(TokenStream));
  }

  SECTION("scanner only rejects") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/marmoset/Je_1_NonJoosConstructs_Unicode.java");
    REQUIRE(!Client.scanOnly(JoosStream));
  }

  SECTION("parser only rejects") {
    std::ifstream TokenStream;
    TokenStream.open(TestDataDir + "/grammar/reject.token");
    REQUIRE(!Client.parseOnly(TokenStream));
  }

  SECTION("preprocessor parser reject") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/marmoset/Je_1_NonJoosConstructs_Volatile.java");
    REQUIRE(!Client.scanOnly(JoosStream));
  }
}
