#include "Weeder.hpp"
#include "catch.hpp"

TEST_CASE("Weeder can parse tree defined rules", "[weeder]") {
  std::unique_ptr<Parse::Node> Node = std::make_unique<Parse::Node>("Root");
  Node->addChild(std::make_unique<Parse::Node>("Child1"));
  Node->addChild(std::make_unique<Parse::Node>("Child2"));
  Parse::Tree Tree(std::move(Node));
  Parse::Weeder Weeder;

  SECTION("Weeder accepts") {
    Parse::Check check1 = [](Parse::Tree &Tree) {
      (void)Tree;
      return true;
    };
    Weeder.addCheck(check1);
    REQUIRE(Weeder.verify(Tree));
  }

  SECTION("Weeder rejects") {
    Parse::Check check1 = [](Parse::Tree &Tree) {
      (void)Tree;
      return true;
    };
    Parse::Check check2 = [](Parse::Tree &Tree) {
      (void)Tree;
      return false;
    };
    Weeder.addCheck(check1);
    Weeder.addCheck(check2);
    REQUIRE(!Weeder.verify(Tree));
  }
}
