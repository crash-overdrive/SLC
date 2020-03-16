#include "ParseTree.hpp"
#include "catch.hpp"
#include <memory>
#include <sstream>
#include <vector>

TEST_CASE("parse tree interacts", "[Parse Tree]") {
  std::unique_ptr<Parse::Node> node = std::make_unique<Parse::Node>("Root");
  node->addChild(std::make_unique<Parse::Node>("Child1"));
  node->addChild(std::make_unique<Parse::Node>("Child2"));
  Parse::Tree tree(std::move(node));

  SECTION("iterator traverses in depth-first search") {
    auto it = tree.begin();
    REQUIRE(it->getName() == "Root");
    REQUIRE(it->getLevel() == 0);
    REQUIRE((++it)->getName() == "Child1");
    REQUIRE(it->getLevel() == 1);
    REQUIRE((++it)->getName() == "Child2");
    REQUIRE(it->getLevel() == 1);
    REQUIRE(++it == tree.end());
  }

  SECTION("tree is printing") {
    std::ostringstream out;
    out << tree;
    REQUIRE(out.str() == "Root\n Child1\n Child2\n");
  }
}
