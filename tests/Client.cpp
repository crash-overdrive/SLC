#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("client process", "[client-process]") {
  std::ifstream ParserStream;
  Parse::DFA Parser;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;
  Client Client;

  SECTION("preprocess step") {
    Client.setBreakPoint(Client::Preprocess);
    SECTION("accept") {
      Client.addJavaFile("good.java");
      REQUIRE(Client.process());
    }
    SECTION("accept with folder") {
      Client.addJavaFile("folder/good.java");
      REQUIRE(Client.process());
    }
    SECTION("reject") {
      Client.addJavaFile("foo.txt");
      REQUIRE_FALSE(Client.process());
    }
    SECTION("reject first dot") {
      Client.addJavaFile("Je_1_ClassDeclaration_WrongFileName_Dot.foo.java");
      REQUIRE_FALSE(Client.process());
    }
  }

  SECTION("basic multiple files") {
    Client.addJavaFiles({
        TestDataDir +
            "/java/a2/J1_3_PackageDecl_SamePackageAndClassName/A/A.java",
        TestDataDir +
            "/java/a2/J1_3_PackageDecl_SamePackageAndClassName/Main.java",
    });
    Client.setStdlib(false);
    REQUIRE(Client.process());
  }

  SECTION("stdlib") {
    Client.addJavaFile(
        TestDataDir +
        "/java/a2/J1_3_PackageExists_AsPrefix_External/Main.java");
    REQUIRE(Client.process());
  }
}
