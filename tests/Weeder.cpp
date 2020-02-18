#include "Weeder.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "ParseDFA.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("weeder detects JoosW", "[weeder]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  auto weedTest = [&](const std::string &Name, const Weed::Check &Check) {
    TokenStream.open(TestDataDir + "/tokens/" + Name);
    Client::parse(Parser, TokenStream);
    return Check(Parser.buildTree());
  };

  SECTION("weeder rejects Abstract and Final in Class") {
    REQUIRE_FALSE(
        weedTest("Je_1_AbstractClass_Final.tokens", Weed::AbstractFinalClass));
  }
}
