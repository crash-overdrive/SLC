#include "ParseTree.hpp"
#include "catch.hpp"
#include <memory>
#include <sstream>
#include <vector>

TEST_CASE("parse tree implementation", "[Parse Tree]") {
  std::unique_ptr<Parse::Node> Node = std::make_unique<Parse::Node>("Root");
  Node->addChild(std::make_unique<Parse::Node>("Child1"));
  Node->addChild(std::make_unique<Parse::Node>("Child2"));
  Parse::Tree Tree(std::move(Node));

  SECTION("iterator traverses in depth-first search") {
    auto it = Tree.begin();
    REQUIRE(it->getName() == "Root");
    REQUIRE(it->getLevel() == 0);
    REQUIRE((++it)->getName() == "Child1");
    REQUIRE(it->getLevel() == 1);
    REQUIRE((++it)->getName() == "Child2");
    REQUIRE(it->getLevel() == 1);
    REQUIRE(++it == Tree.end());
  }

  SECTION("node is able to reference child node by name") {
    auto it = Tree.begin();
    Parse::Node &Node = (*it);
    REQUIRE(Node.find("Child1")->getName() == "Child1");
    REQUIRE(Node.find("Unknown") == nullptr);
  }

  SECTION("tree is able to reference child node by name") {
    auto it = Tree.equalRange("Child2");
    REQUIRE(it.first->second.getName() == "Child2");
  }

  SECTION("tree is printing") {
    std::ostringstream Out;
    Out << Tree;
    REQUIRE(Out.str() == "Root\n Child1\n Child2\n");
  }
}
