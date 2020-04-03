#include "EnvTypeLink.hpp"
#include "TestConfig.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("EnvTypeLink", "[EnvTypeLink]") {
  Env::TypeDeclaration main({}, Env::DeclarationKeyword::Class, "Main");
  Env::TypeDeclaration list({}, Env::DeclarationKeyword::Class, "List",
                            std::make_unique<AST::Start>());
  auto tree = std::make_shared<Env::PackageTree>();

  SECTION("Single DeclarationKeyword Lookup") {
    tree->update({"foo", "bar"}, list);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE(typeLink.find({"foo", "bar", "List"}) == &list);
    REQUIRE(typeLink.addSingleImport({"foo", "bar", "List"}));
    REQUIRE(typeLink.find("List") == &list);
  }

  SECTION("No existing Import") {
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE(typeLink.find(std::vector<std::string>{"nonexist", "class"}) ==
            nullptr);
  }

  SECTION("OnDemand Lookup") {
    tree->update({"foo", "bar"}, list);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    typeLink.addDemandImport({"foo", "bar"});
    REQUIRE(typeLink.find("List") == &list);
  }

  SECTION("Missing entries") {
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE(typeLink.find(std::vector<std::string>{"nonexist", "class"}) ==
            nullptr);
  }

  SECTION("Self Lookup") {
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE(typeLink.find("List") == &list);
  }

  SECTION("Self Import") {
    tree->update({"Test"}, list);
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE(typeLink.addSingleImport({"Test", "List"}));
  }

  SECTION("Multiple Single Import Same") {
    tree->update({"Test"}, list);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE(typeLink.addSingleImport({"Test", "List"}));
    REQUIRE(typeLink.addSingleImport({"Test", "List"}));
  }

  SECTION("Multiple On Demand Import Same") {
    tree->update({"Test"}, list);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE(typeLink.addDemandImport({"Test"}));
    REQUIRE(typeLink.addDemandImport({"Test"}));
  }

  SECTION("Package clash with type") {
    tree->update({"java", "swing"}, list);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    REQUIRE_FALSE(typeLink.addDemandImport({"java", "swing", "List"}));
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("No Single type import equal class same file") {
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "bar", "List"}));
  }

  SECTION("Single import clash") {
    tree->update({"foo", "canary"}, list);
    Env::TypeDeclaration list2({}, Env::DeclarationKeyword::Class, "List",
                               std::make_unique<AST::Start>());
    tree->update({"foo", "bar"}, list2);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    typeLink.addSingleImport({"foo", "bar", "List"});
    REQUIRE_FALSE(typeLink.addSingleImport({"foo", "canary", "List"}));
  }

  SECTION("Same Package Import") {
    Env::TypeDeclaration array({}, Env::DeclarationKeyword::Class, "Array");
    tree->update({"foo", "canary"}, array);
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    typeLink.setPackage({"foo", "canary"});
    REQUIRE(typeLink.find("Array"));
  }

  SECTION("On demand clash") {
    Env::TypeDeclaration list2({}, Env::DeclarationKeyword::Class, "List");
    tree->update({"foo", "canary"}, list);
    tree->update({"foo", "bar"}, list2);
    Env::TypeLink typeLink(main);
    typeLink.setTree(tree);
    typeLink.addDemandImport({"foo", "canary"});
    typeLink.addDemandImport({"foo", "bar"});
    REQUIRE_FALSE(typeLink.find("List"));
  }

  SECTION("Default Package") {
    Env::TypeDeclaration array({}, Env::DeclarationKeyword::Class, "Array");
    tree->update({}, list);
    tree->update({}, array);
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE(typeLink.find("Array"));
  }

  SECTION("Resolve prefix clash default package") {
    Env::TypeDeclaration array({}, Env::DeclarationKeyword::Class, "Array");
    tree->update({"Array"}, list);
    tree->update({}, array);
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    REQUIRE_FALSE(typeLink.find(std::vector<std::string>{"Array", "List"}));
  }

  SECTION("Resolve prefix clash import") {
    Env::TypeDeclaration array({}, Env::DeclarationKeyword::Class, "Array");
    tree->update({"Array"}, list);
    tree->update({"foo", "bar"}, array);
    Env::TypeLink typeLink(list);
    typeLink.setTree(tree);
    typeLink.addSingleImport({"foo", "bar", "Array"});
    REQUIRE_FALSE(typeLink.find(std::vector<std::string>{"Array", "List"}));
  }

  SECTION("Single Import class own name") {
    tree->update({"Test"}, list);
    Env::TypeDeclaration list2({}, Env::DeclarationKeyword::Class, "List",
                               std::make_unique<AST::Start>());
    Env::TypeLink typeLink(list2);
    typeLink.setTree(tree);
    REQUIRE_FALSE(typeLink.addSingleImport({"Test", "List"}));
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
