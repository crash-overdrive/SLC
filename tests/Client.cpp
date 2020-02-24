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

TEST_CASE("client parser detects JoosW", "[client-parser]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  SECTION("parser accepts a1") {
    for (const auto &FileName : A1ValidParserTokens) {
      SECTION(FileName) {
        TokenStream.open(TestDataDir + "/tokens/a1/" + FileName);
        bool status = Client::parse(Parser, TokenStream);
        INFO("This is the parse tree: \n" << Parser.buildTree());
        REQUIRE(status);
      }
    }
  }

  SECTION("parser accepts a2") {
    SECTION("import") {
      TokenStream.open(TestDataDir + "/tokens/a2/J1_classimportMain.tokens");
      INFO("The file is open: " << std::boolalpha << TokenStream.is_open());
      bool status = Client::parse(Parser, TokenStream);
      INFO("This is the parse tree: \n" << Parser.buildTree());
      REQUIRE(status);
    }

    SECTION("package") {
      TokenStream.open(TestDataDir + "/tokens/a2/J1_classimportVector.tokens");
      bool status = Client::parse(Parser, TokenStream);
      INFO("This is the parse tree: \n" << Parser.buildTree());
      REQUIRE(status);
    }
  }

  SECTION("parser rejects") {
    for (const auto &FileName : A1ErrorParserTokens) {
      SECTION(FileName) {
        TokenStream.open(TestDataDir + "/tokens/a1/" + FileName);
        CHECK_FALSE(Client::parse(Parser, TokenStream));
      }
    }
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
