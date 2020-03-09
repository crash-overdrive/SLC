#include "EnvPackageTree.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
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

TEST_CASE("Package Tree", "[PackageTreeLookup][!hide]") {
  Env::FileHeader CanaryHeader({}, Env::TypeDescriptor(Env::Type::Class, "canary"));
  Env::FileHeader BarHeader({}, Env::TypeDescriptor(Env::Type::Class, "bar"));
  Env::PackageTree Tree;
  REQUIRE(Tree.update({"foo", "bar"}, CanaryHeader));
  REQUIRE_FALSE(Tree.update({"foo"}, BarHeader));
  REQUIRE(Tree.lookUp({"foo", "bar", "canary"}) == &CanaryHeader);
  REQUIRE(Tree.lookUp({"foo"}) == nullptr);
}

TEST_CASE("Package Tree Visitor", "[PackageTreeVisitor]") {
  Lex::Scanner Scanner;
  std::ifstream ScannerStream;
  ScannerStream.open(TokensLexFile);
  ScannerStream >> Scanner;

  Parse::DFA Parser;
  std::ifstream ParserStream;
  ParserStream.open(JoosLRFile);
  ParserStream >> Parser;

  Client Client(&Scanner, &Parser);
  SECTION("Recognize Name") {
    std::ifstream JavaStream;
    JavaStream.open(TestDataDir +
                    "/java/a2/J1_3_PackageClashWithType_Linked_Mutated/javax/"
                    "swing/tree/TreeNode.java");
    std::unique_ptr<AST::Start> Root = Client.buildAST(JavaStream);
    Env::PackageTreeVisitor Visitor;
    Root->accept(Visitor);
    REQUIRE(Visitor.getPackagePath() ==
            std::vector<std::string>{"javax", "swing", "tree"});
  }
}
