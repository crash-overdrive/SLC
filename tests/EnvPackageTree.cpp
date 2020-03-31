#include "EnvPackageTree.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("Package Node", "[EnvPackageNode]") {
  Env::PackageNode root{Env::PackageNode::Type::Global};
  Env::PackageNode *foo = root.update(Env::PackageNode::Type::Package, "foo");
  Env::PackageNode *bar = foo->update(Env::PackageNode::Type::Package, "bar");
  bar->update(Env::PackageNode::Type::Declaration, "canary");

  SECTION("Node will reject duplicates") {
    Env::PackageNode *node;
    node = foo->update(Env::PackageNode::Type::Declaration, "bar");
    REQUIRE_FALSE(node);
    node = foo->update(Env::PackageNode::Type::Package, "bar");
    REQUIRE(node);
    node = bar->update(Env::PackageNode::Type::Package, "canary");
    REQUIRE_FALSE(node);
    node = bar->update(Env::PackageNode::Type::Declaration, "canary");
    REQUIRE_FALSE(node);
    node = bar->update(Env::PackageNode::Type::Declaration, "canary");
    REQUIRE_FALSE(node);
  }
}

TEST_CASE("Package Tree", "[EnvPackageTree]") {
  Env::PackageTree tree;

  SECTION("Basic lookup") {
    Env::TypeDeclaration canary({}, Env::DeclarationKeyword::Class, "canary");
    Env::TypeDeclaration bar({}, Env::DeclarationKeyword::Class, "bar");
    REQUIRE(tree.update({"foo", "bar"}, canary));
    REQUIRE_FALSE(tree.update({"foo"}, bar));
    REQUIRE(tree.findDeclaration({"foo", "bar", "canary"}) == &canary);
    REQUIRE_FALSE(tree.findDeclaration({"foo"}));
  }

  SECTION("Default Package lookup") {
    Env::TypeDeclaration canary({}, Env::DeclarationKeyword::Class, "canary");
    REQUIRE(tree.update({}, canary));
    REQUIRE_FALSE(tree.findDeclaration({"canary"}));
    REQUIRE(tree.findDefault({"canary"}) == &canary);
  }

  SECTION("Single File") {
    Env::TypeDeclaration a({}, Env::DeclarationKeyword::Class, "A");
    Env::TypeDeclaration main({}, Env::DeclarationKeyword::Class, "Main");
    REQUIRE(tree.update({"Main", "B"}, a));
    REQUIRE(tree.update({}, main));
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
