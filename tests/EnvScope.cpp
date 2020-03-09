#include "EnvPackageTree.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("hierarchical nodes", "[PackageNode]"){
	Env::PackageNode root{Env::PackageNode::Type::GLOBAL, ""};
  Env::PackageNode *foo = root.update(Env::PackageNode::Type::PACKAGE, "foo");
  Env::PackageNode *bar = foo->update(Env::PackageNode::Type::PACKAGE, "bar");
  bar->update(Env::PackageNode::Type::CLASS, "canary");

	SECTION("Node will reject duplicates") {
    Env::PackageNode *node;
    node = foo->update(Env::PackageNode::Type::CLASS, "bar");
    REQUIRE(node == nullptr);
    node = foo->update(Env::PackageNode::Type::PACKAGE, "bar");
    REQUIRE(node != nullptr);
    node = bar->update(Env::PackageNode::Type::PACKAGE, "canary");
    REQUIRE(node == nullptr);
    node = bar->update(Env::PackageNode::Type::CLASS, "canary");
    REQUIRE(node == nullptr);
    node = bar->update(Env::PackageNode::Type::INTERFACE, "canary");
    REQUIRE(node == nullptr);
	}
}
