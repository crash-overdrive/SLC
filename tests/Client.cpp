#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("client process", "[client]") {
  Lex::Scanner Scanner;
  std::ifstream ScannerStream;
  ScannerStream.open(TokensLexFile);
  ScannerStream >> Scanner;

  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;

  Client Client(Scanner, Parser);

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
      Client.addJavaFile("bar.foo.java");
      REQUIRE_FALSE(Client.process());
    }
  }

  SECTION("a1") {
    SECTION("error preprocess") {
      Client.setBreakPoint(Client::Preprocess);
      for (const auto &FileName : A1ErrorPreprocess) {
        SECTION(FileName) {
          Client.addJavaFile(TestDataDir + "/java/a1/" + FileName);
          REQUIRE_FALSE(Client.process());
        }
      }
    }
    SECTION("error scan") {
      Client.setBreakPoint(Client::Scan);
      for (const auto &FileName : A1ErrorScan) {
        SECTION(FileName) {
          Client.addJavaFile(TestDataDir + "/java/a1/" + FileName);
          REQUIRE_FALSE(Client.process());
        }
      }
    }
    SECTION("error parse") {
      Client.setBreakPoint(Client::Parse);
      for (const auto &FileName : A1ErrorParse) {
        SECTION(FileName) {
          Client.addJavaFile(TestDataDir + "/java/a1/" + FileName);
          REQUIRE_FALSE(Client.process());
        }
      }
    }
    SECTION("accept") {
      Client.setBreakPoint(Client::Weed);
      for (const auto &FileName : A1Valid) {
        SECTION(FileName) {
          Client.addJavaFile(TestDataDir + "/java/a1/" + FileName);
          REQUIRE(Client.process());
        }
      }
    }
  }

  SECTION("a2") {
    SECTION("parse-error") {
      Client.setBreakPoint(Client::Parse);
      for (const auto &Group : A2ErrorParse) {
        SECTION(Group[0]) {
          for (const auto &FileName : Group) {
            Client.addJavaFile(TestDataDir + "/java/a2/" + FileName);
          }
          REQUIRE_FALSE(Client.process());
        }
      }
    }
    SECTION("environment-error") {
      Client.setBreakPoint(Client::TypeLink);
      for (const auto &Group : A2ErrorTyping) {
        SECTION(Group[0]) {
          for (const auto &FileName : Group) {
            Client.addJavaFile(TestDataDir + "/java/a2/" + FileName);
          }
          REQUIRE_FALSE(Client.process());
        }
      }
    }
    // SECTION("typelinking-error") {
    // Client.setBreakPoint(Client::TypeLink);
    // for (const auto &Group : A2Error) {
    // SECTION(Group[0]) {
    // for (const auto &FileName : Group) {
    // Client.addJavaFile(TestDataDir + "/java/a2/" + FileName);
    //}
    // REQUIRE(Client.process());
    //}
    //}
    //}

    SECTION("accept") {
      Client.setBreakPoint(Client::Hierarchy);
      for (const auto &Group : A2Valid) {
        SECTION(Group[0]) {
          for (const auto &FileName : Group) {
            Client.addJavaFile(TestDataDir + "/java/a2/" + FileName);
          }
          REQUIRE(Client.process());
        }
      }
    }
  }

  SECTION("multiple files") {
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
