#include "ParseDFA.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("DFA is able to detect ", "[parse-dfa]") {
  Parse::DFA dfa;
  std::ifstream stream;
  stream.open(testDataDir + "/grammar/sample.lr1");
  stream >> dfa;

  SECTION("DFA rejects") {
    dfa.read({"BOF", "BOF"});
    dfa.read({"id", "3"});
    dfa.read({"-", "-"});
    REQUIRE_FALSE(dfa.accept());
    REQUIRE_FALSE(dfa.error());
    dfa.read({"EOF", "EOF"});
    REQUIRE(dfa.error());
    REQUIRE_FALSE(dfa.accept());
  }

  SECTION("DFA accepts basic") {
    dfa.read({"BOF", "BOF"});
    dfa.read({"id", "2"});
    dfa.read({"EOF", "EOF"});
    REQUIRE(dfa.accept());
    REQUIRE_FALSE(dfa.error());

    Parse::Tree t = dfa.buildTree();
    std::ostringstream osStream;
    osStream << t;

    std::ifstream treeStream(testDataDir + "/grammar/sample.tree");
    std::string treeString(std::istreambuf_iterator<char>(treeStream), {});
    REQUIRE(treeString == osStream.str());
  }

  SECTION("DFA accepts complex") {
    std::vector<Lex::Token> v{
        {"BOF", "BOF"}, {"id", "3"}, {"-", "-"}, {"(", "("},     {"id", "5"},
        {"-", "-"},     {"id", "4"}, {")", ")"}, {"EOF", "EOF"},
    };
    for (const auto &tok : v) {
      dfa.read(tok);
    }
    REQUIRE(dfa.accept());
    REQUIRE_FALSE(dfa.error());
  }

  SECTION("DFA reject complex") {
    std::vector<Lex::Token> v{
        {"BOF", "BOF"}, {"id", "3"}, {"-", "-"},  {"(", "("},
        {"id", "5"},    {"-", "-"},  {"id", "4"}, {"id", "3"},
    };
    for (const auto &tok : v) {
      dfa.read(tok);
    }
    REQUIRE_FALSE(dfa.accept());
    REQUIRE(dfa.error());
  }
}

TEST_CASE("parser detects Java", "[parse-java]") {
  Parse::DFA parser;
  std::ifstream parserStream;
  parserStream.open(joosLRFile);
  parserStream >> parser;
  std::ifstream tokenStream;

  SECTION("parser accepts a2") {
    SECTION("import") {
      tokenStream.open(testDataDir + "/tokens/a2/J1_classimportMain.tokens");
      INFO("The file is open: " << std::boolalpha << tokenStream.is_open());
      bool status = parser.parse(tokenStream);
      INFO("This is the parse tree: \n" << parser.buildTree());
      REQUIRE(status);
    }

    SECTION("package") {
      tokenStream.open(testDataDir + "/tokens/a2/J1_classimportVector.tokens");
      bool status = parser.parse(tokenStream);
      INFO("This is the parse tree: \n" << parser.buildTree());
      REQUIRE(status);
    }
  }
}
