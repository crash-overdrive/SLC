#include "EnvPackageTree.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("Package Node", "[PackageNode]") {
  Env::PackageNode root{Env::PackageNode::Type::Global};
  Env::PackageNode *foo = root.update(Env::PackageNode::Type::Package, "foo");
  Env::PackageNode *bar = foo->update(Env::PackageNode::Type::Package, "bar");
  bar->update(Env::PackageNode::Type::JoosType, "canary");

  SECTION("Node will reject duplicates") {
    Env::PackageNode *node;
    node = foo->update(Env::PackageNode::Type::JoosType, "bar");
    REQUIRE(node == nullptr);
    node = foo->update(Env::PackageNode::Type::Package, "bar");
    REQUIRE(node != nullptr);
    node = bar->update(Env::PackageNode::Type::Package, "canary");
    REQUIRE(node == nullptr);
    node = bar->update(Env::PackageNode::Type::JoosType, "canary");
    REQUIRE(node == nullptr);
    node = bar->update(Env::PackageNode::Type::JoosType, "canary");
    REQUIRE(node == nullptr);
  }
}

TEST_CASE("Package Tree", "[PackageTreeLookup]]") {
  Env::PackageTree tree;

  SECTION("Basic lookup") {
    Env::Hierarchy canary(Env::FileHeader({}, {Env::Type::Class, "canary"}));
    Env::Hierarchy bar(Env::FileHeader({}, {Env::Type::Class, "bar"}));
    REQUIRE(tree.update({"foo", "bar"}, canary));
    REQUIRE_FALSE(tree.update({"foo"}, bar));
    REQUIRE(tree.findHierarchy({"foo", "bar", "canary"}) == &canary);
    REQUIRE(tree.findHierarchy({"foo"}) == nullptr);
  }

  SECTION("Single File") {
    Env::Hierarchy aHier(Env::FileHeader({}, {Env::Type::Class, "A"}));
    Env::Hierarchy mainHier(Env::FileHeader({}, {Env::Type::Class, "Main"}));
    REQUIRE(tree.update({"Main", "B"}, aHier));
    REQUIRE(tree.update({}, mainHier));
  }
}

TEST_CASE("Package Tree Visitor", "[PackageTreeVisitor]") {
  Client client = createClient();
  std::unique_ptr<AST::Start> root = client.buildAST(
      testDataDir + "/java/a2/J1_3_PackageClashWithType_Linked_Mutated/javax/"
                    "swing/tree/TreeNode.java");
  Env::PackageTreeVisitor visitor;
  root->accept(visitor);
  REQUIRE(visitor.getPackagePath() ==
          std::vector<std::string>{"javax", "swing", "tree"});
}
