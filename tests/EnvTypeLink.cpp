#include "EnvTypeLink.hpp"
#include "EnvFileHeader.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("EnvTypeLink", "[EnvTypeLink]") {
  Env::Hierarchy mainHier(Env::FileHeader({}, {Env::Type::Class, "Main"}));
  Env::Hierarchy listHier(Env::FileHeader({}, {Env::Type::Class, "List"},
                                          std::make_unique<AST::Start>()));
  auto tree = std::make_shared<Env::PackageTree>();

  SECTION("Single Type Lookup") {
    tree->update({"foo", "bar"}, listHier);
    Env::TypeLink typeLink(mainHier, tree);
    REQUIRE(typeLink.find({"foo", "bar", "List"}) == &listHier);
    REQUIRE(typeLink.addSingleImport({"foo", "bar", "List"}));
    REQUIRE(typeLink.find({"List"}) == &listHier);
  }

  SECTION("No existing Import") {
    Env::TypeLink typeLink(mainHier, tree);
    REQUIRE(typeLink.find({"nonexist", "class"}) == nullptr);
  }

  SECTION("OnDemand Lookup") {
    tree->update({"foo", "bar"}, listHier);
    Env::TypeLink typeLink(mainHier, tree);
    typeLink.addDemandImport({"foo", "bar"});
    REQUIRE(typeLink.find({"List"}) == &listHier);
  }

  SECTION("Missing entries") {
    Env::TypeLink typeLink(mainHier, tree);
    REQUIRE(typeLink.find({"nonexist", "class"}) == nullptr);
  }

  SECTION("Self Lookup") {
    Env::TypeLink typeLink(listHier, tree);
    REQUIRE(typeLink.find({"List"}) == &listHier);
  }

  SECTION("Self Import") {
    tree->update({"Test"}, listHier);
    Env::TypeLink typeLink(listHier, tree);
    REQUIRE(typeLink.addSingleImport({"Test", "List"}));
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(listHier, tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(listHier, tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("Single import clash") {
    tree->update({"foo", "canary"}, listHier);
    Env::Hierarchy listHier(Env::FileHeader({}, {Env::Type::Class, "List"},
                                            std::make_unique<AST::Start>()));
    tree->update({"foo", "bar"}, listHier);
    Env::TypeLink typeLink(mainHier, tree);
    typeLink.addSingleImport({"foo", "bar", "List"});
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "canary", "List"}));
  }

  SECTION("Same Package Import") {
    Env::Hierarchy arrayHier(Env::FileHeader({}, {Env::Type::Class, "Array"}));
    tree->update({"foo", "canary"}, listHier);
    tree->update({"foo", "canary"}, arrayHier);
    Env::TypeLink typeLink(listHier, tree);
    REQUIRE(typeLink.find({"Array"}));
  }
}

TEST_CASE("EnvTypeLink Visitor", "[EnvTypeLinkVisitor]") {
  Client client = createClient();

  SECTION("Single Import and Demand") {
    std::unique_ptr<AST::Start> root = client.buildAST(
        testDataDir + "/java/a2/J1_importNameLookup1/Main.java");
    Env::TypeLinkVisitor visitor;
    root->accept(visitor);
    REQUIRE(*visitor.getSingleImports().begin() ==
            std::vector<std::string>{"baz", "foo"});
    REQUIRE(*visitor.getDemandImports().begin() ==
            std::vector<std::string>{"bar"});
  }

  SECTION("Single Import and Demand") {
    std::unique_ptr<AST::Start> root = client.buildAST(
        testDataDir +
        "/java/a2/J1_3_SingleTypeImport_MultipleFromSamePackage/Main.java");
    Env::TypeLinkVisitor visitor;
    root->accept(visitor);
    REQUIRE(visitor.getSingleImports().at(0) ==
            std::vector<std::string>{"java", "util", "List"});
    REQUIRE(visitor.getSingleImports().at(1) ==
            std::vector<std::string>{"java", "util", "LinkedList"});
  }
}
