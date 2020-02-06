#include "ParseTree.hpp"
#include "catch.hpp"
#include <memory>
#include <sstream>
#include <vector>

TEST_CASE("Parse tree implementation", "[Parse Tree]") {
  std::unique_ptr<Parse::Node> Node = std::make_unique<Parse::Node>("Root");
  Node->addChild(std::make_unique<Parse::Node>("Child1"));
  Node->addChild(std::make_unique<Parse::Node>("Child2"));
  Parse::Tree Tree(std::move(Node));

  SECTION("Iterator traverse in depth-first search") {
    auto it = Tree.begin();
    REQUIRE((*it).getName() == "Root");
    REQUIRE((*it).getLevel() == 0);
    REQUIRE((*++it).getName() == "Child1");
    REQUIRE((*it).getLevel() == 1);
    REQUIRE((*++it).getName() == "Child2");
    REQUIRE((*it).getLevel() == 1);
    REQUIRE(++it == Tree.end());
  }

  SECTION("Printing in Tree format") {
    std::ostringstream Out;
    Out << Tree;
    REQUIRE(Out.str() == "Root\n Child1\n Child2\n");
  }
}
