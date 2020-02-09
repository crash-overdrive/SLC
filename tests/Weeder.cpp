#include "Config.hpp"
#include "TestConfig.hpp"
#include "ParseDFA.hpp"
#include "Weeder.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("weeder can reject non-Joos", "[weeder]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  auto weedTest = [&](const std::string &Name, const Weed::Check &Check) {
    TokenStream.open(TestDataDir + "/grammar/" + Name);
    Parse::Tree Tree = Parser.buildTree();
    return Check(Tree);
  };

  SECTION("weeder rejects Abstract and Final in Class") {
    REQUIRE(!weedTest("J1_publicclasses.token", Weed::AbstractFinalClass));
  }
}
