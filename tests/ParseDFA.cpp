#include "ParseDFA.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("DFA is able to detect ", "[parse-dfa]") {
  Parse::DFA DFA;
  std::ifstream Stream;
  Stream.open(TestDataDir + "/grammar/sample.lr1");
  DFA.configure(Stream);

  SECTION("DFA rejects") {
    DFA.read("BOF");
    DFA.read("id");
    DFA.read("-");
    REQUIRE(!DFA.accept());
    REQUIRE(!DFA.error());
    DFA.read("EOF");
    REQUIRE(DFA.error());
    REQUIRE(!DFA.accept());
  }

  SECTION("DFA accepts basic") {
    DFA.read("BOF");
    DFA.read("id");
    DFA.read("EOF");
    REQUIRE(DFA.accept());
    REQUIRE(!DFA.error());

    Parse::Tree T = DFA.buildTree();
    std::ostringstream Oss;
    Oss << T;
    std::ifstream TestTree(TestDataDir + "/grammar/sample.tree");
    std::ostringstream OssFile;
    OssFile << TestTree.rdbuf();
    REQUIRE(Oss.str() == OssFile.str());
  }

  SECTION("DFA accepts complex") {
    std::vector<std::string> V{"BOF", "id", "-", "(",  "id",
                               "-",   "id", ")", "EOF"};
    for (const auto &Token : V) {
      DFA.read(Token);
    }
    REQUIRE(DFA.accept());
    REQUIRE(!DFA.error());
  }

  SECTION("DFA reject complex") {
    std::vector<std::string> V{"BOF", "id", "-", "(", "id",
                               "-",   "id", ")", "id"};
    for (const auto &Token : V) {
      DFA.read(Token);
    }
    REQUIRE(!DFA.accept());
    REQUIRE(DFA.error());
  }
}
