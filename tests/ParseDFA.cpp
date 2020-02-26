#include "ParseDFA.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("DFA is able to detect ", "[parse-dfa]") {
  Parse::DFA DFA;
  std::ifstream Stream;
  Stream.open(TestDataDir + "/grammar/sample.lr1");
  Stream >> DFA;

  SECTION("DFA rejects") {
    DFA.read({"BOF", "BOF"});
    DFA.read({"id", "3"});
    DFA.read({"-", "-"});
    REQUIRE_FALSE(DFA.accept());
    REQUIRE_FALSE(DFA.error());
    DFA.read({"EOF", "EOF"});
    REQUIRE(DFA.error());
    REQUIRE_FALSE(DFA.accept());
  }

  SECTION("DFA accepts basic") {
    DFA.read({"BOF", "BOF"});
    DFA.read({"id", "2"});
    DFA.read({"EOF", "EOF"});
    REQUIRE(DFA.accept());
    REQUIRE_FALSE(DFA.error());

    Parse::Tree T = DFA.buildTree();
    std::ostringstream OSStream;
    OSStream << T;

    std::ifstream TreeStream(TestDataDir + "/grammar/sample.tree");
    std::string TreeString(std::istreambuf_iterator<char>(TreeStream), {});
    REQUIRE(TreeString == OSStream.str());
  }

  SECTION("DFA accepts complex") {
    std::vector<Lex::Token> V{
        {"BOF", "BOF"}, {"id", "3"}, {"-", "-"}, {"(", "("},     {"id", "5"},
        {"-", "-"},     {"id", "4"}, {")", ")"}, {"EOF", "EOF"},
    };
    for (const auto &Tok : V) {
      DFA.read(Tok);
    }
    REQUIRE(DFA.accept());
    REQUIRE_FALSE(DFA.error());
  }

  SECTION("DFA reject complex") {
    std::vector<Lex::Token> V{
        {"BOF", "BOF"}, {"id", "3"}, {"-", "-"},  {"(", "("},
        {"id", "5"},    {"-", "-"},  {"id", "4"}, {"id", "3"},
    };
    for (const auto &Tok : V) {
      DFA.read(Tok);
    }
    REQUIRE_FALSE(DFA.accept());
    REQUIRE(DFA.error());
  }
}

TEST_CASE("parser detects Java", "[parse-java]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  SECTION("parser accepts a2") {
    SECTION("import") {
      TokenStream.open(TestDataDir + "/tokens/a2/J1_classimportMain.tokens");
      INFO("The file is open: " << std::boolalpha << TokenStream.is_open());
      bool status = Parser.parse(TokenStream);
      INFO("This is the parse tree: \n" << Parser.buildTree());
      REQUIRE(status);
    }

    SECTION("package") {
      TokenStream.open(TestDataDir + "/tokens/a2/J1_classimportVector.tokens");
      bool status = Parser.parse(TokenStream);
      INFO("This is the parse tree: \n" << Parser.buildTree());
      REQUIRE(status);
    }
  }
}
