#include "EnvTypeLink.hpp"
#include "EnvFileHeader.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("EnvTypeLink", "[EnvTypeLink]") {
  Env::FileHeader mainHeader({}, {Env::Type::Class, "Main"});
  Env::FileHeader listHeader({}, {Env::Type::Class, "List"},
                             std::make_unique<AST::Start>());
  Env::PackageTree tree;

  SECTION("Simple Lookup") {
    tree.update({"foo", "bar"}, listHeader);
    Env::TypeLink typeLink(mainHeader, tree);
    REQUIRE(typeLink.find({"foo", "bar", "List"}) == &listHeader);
    REQUIRE(typeLink.addSingleImport({"foo", "bar", "List"}));
    REQUIRE(typeLink.find({"List"}) == &listHeader);
  }

  SECTION("Import") {
    Env::TypeLink typeLink(mainHeader, tree);
    REQUIRE(typeLink.find({"nonexist", "class"}) == nullptr);
  }

  SECTION("Missing entries") {
    Env::TypeLink typeLink(mainHeader, tree);
    REQUIRE(typeLink.find({"nonexist", "class"}) == nullptr);
  }

  SECTION("Self Lookup") {
    Env::TypeLink typeLink(listHeader, tree);
    REQUIRE(typeLink.find({"List"}) == &listHeader);
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(listHeader, tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(listHeader, tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("Single import clash") {
    tree.update({"foo", "canary"}, listHeader);
    Env::FileHeader list2Header({}, {Env::Type::Class, "List"},
                                std::make_unique<AST::Start>());
    tree.update({"foo", "bar"}, list2Header);
    Env::TypeLink typeLink(mainHeader, tree);
    typeLink.addSingleImport({"foo", "bar", "List"});
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "canary", "List"}));
  }

  SECTION("Same Package Import") {
    Env::FileHeader arrayHeader({}, {Env::Type::Class, "Array"});
    tree.update({"foo", "canary"}, listHeader);
    tree.update({"foo", "canary"}, arrayHeader);
    Env::TypeLink typeLink(listHeader, tree);
    REQUIRE(typeLink.find({"Array"}));
  }

  SECTION("On-demand clash") {
    tree.update({"foo", "canary"}, listHeader);
    Env::TypeLink typeLink(mainHeader, tree);
    typeLink.addDemandImport({"foo"});
    REQUIRE_FALSE(typeLink.find({"List"}) == nullptr);
  }
}

TEST_CASE("EnvTypeLink Visitor", "[EnvTypeLinkVisitor]") {
  Client Client = createClient();

  SECTION("Single Import and Demand") {
    std::unique_ptr<AST::Start> Root = Client.buildAST(
        TestDataDir + "/java/a2/J1_importNameLookup1/Main.java");
    Env::TypeLinkVisitor Visitor;
    Root->accept(Visitor);
    REQUIRE(*Visitor.getSingleImports().begin() ==
            std::vector<std::string>{"baz", "foo"});
    REQUIRE(*Visitor.getDemandImports().begin() ==
            std::vector<std::string>{"bar"});
  }

  SECTION("Single Import and Demand") {
    std::unique_ptr<AST::Start> Root = Client.buildAST(
        TestDataDir +
        "/java/a2/J1_3_SingleTypeImport_MultipleFromSamePackage/Main.java");
    Env::TypeLinkVisitor Visitor;
    Root->accept(Visitor);
    REQUIRE(Visitor.getSingleImports().at(0) ==
            std::vector<std::string>{"java", "util", "List"});
    REQUIRE(Visitor.getSingleImports().at(1) ==
            std::vector<std::string>{"java", "util", "LinkedList"});
  }
}
