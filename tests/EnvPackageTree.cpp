#include "EnvPackageTree.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("hierarchical nodes", "[PackageNode]") {
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
