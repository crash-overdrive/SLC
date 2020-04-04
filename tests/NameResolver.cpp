#include "NameResolver.hpp"
#include "catch.hpp"

TEST_CASE("Resolve names", "[NameResolver]") {
  Env::TypeDeclaration listDecl({}, Env::DeclarationKeyword::Class, "List");
  Env::Type listType(&listDecl);
  Env::TypeDeclaration arrayDecl({}, Env::DeclarationKeyword::Class, "Array");
  Env::Type arrayType(&arrayDecl);

  auto tree = std::make_shared<Env::PackageTree>();
  Env::TypeLink typeLink(listDecl, tree);
  typeLink.setPackage({});
  Env::Local local;
  Name::Resolver resolver(local, typeLink, listDecl);

  SECTION("Single Identifier") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    listDecl.contain.addDeclareField(&field);
    REQUIRE(*resolver.findField({"foo"}) == Env::TypeKeyword::Integer);

    local.addVariable("foo", Env::TypeKeyword::Boolean);
    REQUIRE(*resolver.findField({"foo"}) == Env::TypeKeyword::Boolean);
  }

  SECTION("Static cannot resolve to non-static") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    listDecl.contain.addDeclareField(&field);
    REQUIRE_FALSE(resolver.findField({"List", "foo"}));
  }

  SECTION("Static field can be called") {
    Env::Field field({Env::Modifier::Static}, Env::TypeKeyword::Integer, "foo");
    listDecl.contain.addDeclareField(&field);
    REQUIRE(resolver.findField({"List", "foo"}));
  }

  SECTION("Local Variable field") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    listDecl.contain.addDeclareField(&field);
    local.addVariable("list", listType);
    REQUIRE(resolver.findField({"list", "foo"}));
  }

  SECTION("Static cannot call non-static") {
    Env::Method method({}, Env::TypeKeyword::Integer, "foo", {});
    listDecl.contain.addDeclareMethod(&method);
    REQUIRE_FALSE(resolver.findMethod({"List", "foo"}, {}));
  }

  SECTION("Static call") {
    Env::Method method({Env::Modifier::Static}, Env::TypeKeyword::Integer,
                       "foo", {});
    listDecl.contain.addDeclareMethod(&method);
    REQUIRE(resolver.findMethod({"List", "foo"}, {}));
  }

  SECTION("Same class protected") {
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    listDecl.contain.addDeclareField(&field);
    local.addVariable("list", listType);
    REQUIRE(resolver.findField({"list", "foo"}));
  }

  SECTION("Same package protected") {
    typeLink.setPackage({"bar"});
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);

    local.addVariable("array", arrayType);
    REQUIRE(resolver.findField({"array", "foo"}));
  }

  SECTION("Subclass protected") {
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    arrayDecl.contain.addDeclareField(&field);
    listDecl.subType.emplace(&arrayDecl);
    local.addVariable("array", arrayType);
    REQUIRE(resolver.findField({"array", "foo"}));
  }

  SECTION("World cannot see protected") {
    Env::Field field({Env::Modifier::Static, Env::Modifier::Protected},
                     Env::TypeKeyword::Integer, "foo");
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);
    REQUIRE_FALSE(resolver.findField({"bar", "Array", "foo"}));
  }

  SECTION("World can see public") {
    Env::Field field({Env::Modifier::Static, Env::Modifier::Public},
                     Env::TypeKeyword::Integer, "foo");
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);
    REQUIRE(resolver.findField({"bar", "Array", "foo"}));
  }

  SECTION("Cannot construct object of abstract class") {
    Env::TypeDeclaration baseDecl({Env::Modifier::Abstract},
                                  Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Public}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE_FALSE(resolver.findConstructor({"bar", "Base"}, {}));
  }

  SECTION("World cannot use protected constructor") {
    Env::TypeDeclaration baseDecl({}, Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Protected}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE_FALSE(resolver.findConstructor({"bar", "Base"}, {}));
  }

  SECTION("Found Constructor") {
    Env::TypeDeclaration baseDecl({Env::Modifier::Public},
                                  Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Public}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE(resolver.findConstructor({"bar", "Base"}, {}));
  }
}
