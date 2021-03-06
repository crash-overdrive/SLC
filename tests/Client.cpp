#include "MarmosetConfig.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("client process", "[client]") {
  Client client = createClient();

  SECTION("preprocess step") {
    client.setBreakPoint(Client::VerifyName);
    SECTION("accept") { REQUIRE(client.compile({"good.java"})); }
    SECTION("accept with folder") {
      REQUIRE(client.compile({"folder/good.java"}));
    }
    SECTION("reject") { REQUIRE_FALSE(client.compile({"foo.txt"})); }
    SECTION("reject no dot ") { REQUIRE_FALSE(client.compile({"foooo"})); }
    SECTION("reject first dot") {
      REQUIRE_FALSE(client.compile({"bar.foo.java"}));
    }
  }

  SECTION("a1") {
    SECTION("error-verifyName") {
      client.setBreakPoint(Client::VerifyName);
      for (const auto &fileName : a1ErrorVerifyName) {
        SECTION(fileName) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(1, {fileName})));
        }
      }
    }

    SECTION("error-scan") {
      client.setBreakPoint(Client::Scan);
      for (const auto &fileName : a1ErrorScan) {
        SECTION(fileName) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(1, {fileName})));
        }
      }
    }

    SECTION("error-parse") {
      client.setBreakPoint(Client::Parse);
      for (const auto &fileName : a1ErrorParse) {
        SECTION(fileName) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(1, {fileName})));
        }
      }
    }

    SECTION("error-weed") {
      client.setBreakPoint(Client::Weed);
      for (const auto &fileName : a1ErrorWeed) {
        SECTION(fileName) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(1, {fileName})));
        }
      }
    }

    SECTION("accept") {
      for (const auto &fileName : a1Valid) {
        SECTION(fileName) {
          REQUIRE(client.compile(createMarmosetTest(1, {fileName})));
        }
      }
    }
  }

  SECTION("a2") {
    SECTION("error-parse") {
      client.setBreakPoint(Client::Parse);
      for (const auto &group : a2ErrorParse) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("error-localvariable") {
      client.setBreakPoint(Client::LocalVariable);
      for (const auto &group : a2ErrorLocal) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("error-packagetree") {
      client.setBreakPoint(Client::PackageTree);
      for (const auto &group : a2ErrorPackageTree) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("error-typeLink") {
      client.setBreakPoint(Client::TypeLink);
      for (const auto &group : a2ErrorTypeLink) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("error-typebody") {
      client.setBreakPoint(Client::TypeBody);
      for (const auto &group : a2ErrorTypeBody) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("error-hierarchy") {
      client.setBreakPoint(Client::Hierarchy);
      for (const auto &group : a2ErrorHierarchy) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }

    SECTION("accept") {
      for (const auto &group : a2Valid) {
        SECTION(group[0]) {
          REQUIRE(client.compile(createMarmosetTest(2, group)));
        }
      }
    }
  }

  SECTION("a3") {
    SECTION("accept") {
      for (const auto &group : a3Valid) {
        SECTION(group[0]) {
          REQUIRE(client.compile(createMarmosetTest(3, group)));
        }
      }
    }

    SECTION("parse-error") {
      client.setBreakPoint(Client::Parse);
      for (const auto &group : a3ErrorParse) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(3, group)));
        }
      }
    }

    SECTION("local-error") {
      client.setBreakPoint(Client::LocalVariable);
      for (const auto &group : a3ErrorLocal) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(3, group)));
        }
      }
    }

    SECTION("typecheck-error") {
      client.setBreakPoint(Client::TypeCheck);
      for (const auto &group : a3ErrorTypeCheck) {
        SECTION(group[0]) {
          REQUIRE_FALSE(client.compile(createMarmosetTest(3, group)));
        }
      }
    }

    SECTION("error") {
      for (const auto &group : a3Error) {
        SECTION(group[0]) {
          CHECK(client.compile(createMarmosetTest(3, group)));
        }
      }
    }
  }

  SECTION("a4") {
    SECTION("accept") {
      for (const auto &group : a4Valid) {
        SECTION(group[0]) {
          REQUIRE(client.compile(createMarmosetTest(4, group)));
        }
      }
    }
  }
}
