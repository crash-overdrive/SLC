#include "EnvTypeLink.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

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
