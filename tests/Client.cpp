#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("client process", "[client]") {
  Client Client = createClient();

  SECTION("preprocess step") {
    Client.setBreakPoint(Client::VerifyName);
    SECTION("accept") { REQUIRE(Client.compile({"good.java"})); }
    SECTION("accept with folder") {
      REQUIRE(Client.compile({"folder/good.java"}));
    }
    SECTION("reject") { REQUIRE_FALSE(Client.compile({"foo.txt"})); }
    SECTION("reject first dot") {
      REQUIRE_FALSE(Client.compile({"bar.foo.java"}));
    }
  }

  SECTION("a1") {
    SECTION("error VerifyName") {
      Client.setBreakPoint(Client::VerifyName);
      for (const auto &FileName : A1ErrorPreprocess) {
        SECTION(FileName) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(1, {FileName})));
        }
      }
    }

    SECTION("error scan") {
      Client.setBreakPoint(Client::Scan);
      for (const auto &FileName : A1ErrorScan) {
        SECTION(FileName) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(1, {FileName})));
        }
      }
    }

    SECTION("error parse") {
      Client.setBreakPoint(Client::Parse);
      for (const auto &FileName : A1ErrorParse) {
        SECTION(FileName) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(1, {FileName})));
        }
      }
    }

    SECTION("accept") {
      for (const auto &FileName : A1Valid) {
        SECTION(FileName) {
          REQUIRE(Client.compile(createMarmosetTest(1, {FileName})));
        }
      }
    }
  }

  SECTION("a2") {
    SECTION("parse-error") {
      Client.setBreakPoint(Client::Parse);
      for (const auto &Group : A2ErrorParse) {
        SECTION(Group[0]) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(2, Group)));
        }
      }
    }

    SECTION("fileheader-error") {
      Client.setBreakPoint(Client::FileHeader);
      for (const auto &Group : A2ErrorFileHeader) {
        SECTION(Group[0]) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(2, Group)));
        }
      }
    }

    SECTION("package-error") {
      Client.setBreakPoint(Client::PackageTree);
      for (const auto &Group : A2ErrorPackage) {
        SECTION(Group[0]) {
          REQUIRE_FALSE(Client.compile(createMarmosetTest(2, Group)));
        }
      }
    }
    // SECTION("reject") {
    // for (const auto &Group : A2Error) {
    // SECTION(Group[0]) {
    // for (const auto &FileName : Group) {
    // Client.addJavaFile(TestDataDir + "/java/a2/" + FileName);
    //}
    // REQUIRE(Client.compile());
    //}
    //}
    //}

    SECTION("accept") {
      for (const auto &Group : A2Valid) {
        SECTION(Group[0]) {
          REQUIRE(Client.compile(createMarmosetTest(2, Group)));
        }
      }
    }

    SECTION("a3") {
      SECTION("accept") {
        for (const auto &Group : A3Valid) {
          SECTION(Group[0]) {
            REQUIRE(Client.compile(createMarmosetTest(3, Group)));
          }
        }
      }

      SECTION("parse-error") {
        Client.setBreakPoint(Client::Parse);
        for (const auto &Group : A3ErrorParse) {
          SECTION(Group[0]) {
            REQUIRE_FALSE(Client.compile(createMarmosetTest(3, Group)));
          }
        }
      }
    }

    SECTION("a4") {
      SECTION("accept") {
        for (const auto &Group : A4Valid) {
          SECTION(Group[0]) {
            REQUIRE(Client.compile(createMarmosetTest(4, Group)));
          }
        }
      }
    }

    SECTION("multiple files") {
      std::vector<std::string> files{
          TestDataDir +
              "/java/a2/J1_3_PackageDecl_SamePackageAndClassName/A/A.java",
          TestDataDir +
              "/java/a2/J1_3_PackageDecl_SamePackageAndClassName/Main.java",
      };
      REQUIRE(Client.compile(files));
    }

    SECTION("stdlib") {
      std::vector<std::string> files{
          TestDataDir +
          "/java/a2/J1_3_PackageExists_AsPrefix_External/Main.java"};
      REQUIRE(Client.compile(files));
    }
  }
}
