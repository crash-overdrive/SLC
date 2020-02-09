#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("client detects extension error JoosW", "[client-filename]") {
  REQUIRE(!Client::verifyFileName(
      "Je_1_ClassDeclaration_WrongFileName_Dot.foo.java"));
  REQUIRE(!Client::verifyFileName("foo.txt"));
  REQUIRE(Client::verifyFileName("good.java"));
}

TEST_CASE("client scanner detects JoosW", "[client-scanner]") {
  SECTION("scanner accept") {
    std::ostringstream OStream;
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir + "/marmoset/J1_publicclasses.java");
    REQUIRE(Client::scan(JoosStream, OStream));

    std::ifstream TokenStream;
    TokenStream.open(TestDataDir + "/grammar/J1_publicclasses.token");
    std::string TokenString(std::istreambuf_iterator<char>(TokenStream), {});
    REQUIRE(TokenString == OStream.str());
  }

  SECTION("scanner rejects") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/marmoset/Je_1_NonJoosConstructs_Unicode.java");
    REQUIRE(!Client::scan(JoosStream));
  }
}

TEST_CASE("client parser detects JoosW", "[client-parser]") {
  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  std::ifstream TokenStream;

  SECTION("parser accept") {
    std::vector<std::string> FileNames{"J1_abstractclass.token",
                                       "J1_publicclasses.token"};
    for (const auto &FileName : FileNames) {
      TokenStream.open(TestDataDir + "/grammar/" + FileName);
      REQUIRE(Client::parse(Parser, TokenStream));
      std::cout << Parser.buildTree();
      TokenStream.close();
      TokenStream.clear();
      Parser.clear();
    }
  }

  SECTION("parser rejects") {
    std::vector<std::string> FileNames{"SimpleReject.token",
                                       "MissingClassBody.token"};
    for (const auto &FileName : FileNames) {
      TokenStream.open(TestDataDir + "/grammar/" + FileName);
      REQUIRE(!Client::parse(Parser, TokenStream));
      TokenStream.close();
      TokenStream.clear();
      Parser.clear();
    }
  }
}

TEST_CASE("client preprocessor detects JoosW", "[client-preprocess]") {
  SECTION("preprocessor parser reject") {
    std::ifstream JoosStream;
    JoosStream.open(TestDataDir +
                    "/marmoset/Je_1_NonJoosConstructs_Volatile.java");
    REQUIRE(!Client::preProcess(JoosStream,
                                "Je_1_NonJoosConstructs_Volatile.java"));
  }
}
