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
  Name::ResolverFactory Resolver(local, typeLink);
  Name::FieldResolver fieldResolver = Resolver.getField();
  Name::MethodResolver methodResolver = Resolver.getMethod();
  Name::ConstructorResolver constructorResolver = Resolver.getConstructor();

  SECTION("Single Identifier") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    field.declaration = &listDecl;
    listDecl.contain.addDeclareField(&field);
    REQUIRE(fieldResolver.match({"foo"}) == Env::TypeKeyword::Integer);

    local.addVariable("foo", Env::TypeKeyword::Boolean);
    Name::FieldResolver fieldResolver2 = Resolver.getField();
    REQUIRE(fieldResolver2.match({"foo"}) == Env::TypeKeyword::Boolean);
  }

  SECTION("Static cannot resolve to non-static") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    field.declaration = &listDecl;
    listDecl.contain.addDeclareField(&field);
    REQUIRE_FALSE(fieldResolver.match({"List", "foo"}));
  }

  SECTION("Static field can be called") {
    Env::Field field({Env::Modifier::Static}, Env::TypeKeyword::Integer, "foo");
    field.declaration = &listDecl;
    listDecl.contain.addDeclareField(&field);
    REQUIRE(fieldResolver.match({"List", "foo"}));
  }

  SECTION("Static field cannot be called implicit") {
    Env::Field field({Env::Modifier::Static}, Env::TypeKeyword::Integer, "foo");
    field.declaration = &listDecl;
    listDecl.contain.addDeclareField(&field);
    REQUIRE_FALSE(fieldResolver.match({"foo"}));
  }

  SECTION("Local Variable field") {
    Env::Field field({}, Env::TypeKeyword::Integer, "foo");
    listDecl.contain.addDeclareField(&field);
    local.addVariable("list", listType);
    REQUIRE(fieldResolver.match({"list", "foo"}));
  }

  SECTION("Static cannot call non-static") {
    Env::Method method({}, Env::TypeKeyword::Integer, "foo", {});
    listDecl.contain.addDeclareMethod(&method);
    REQUIRE_FALSE(methodResolver.match({"List", "foo"}));
  }

  SECTION("Static cannot be called implicitly") {
    Env::Method method({Env::Modifier::Static}, Env::TypeKeyword::Integer,
                       "foo", {});
    listDecl.contain.addDeclareMethod(&method);
    REQUIRE_FALSE(methodResolver.match({"foo"}));
  }

  SECTION("Static call") {
    Env::Method method({Env::Modifier::Static}, Env::TypeKeyword::Integer,
                       "foo", {});
    method.declaration = &listDecl;
    listDecl.contain.addDeclareMethod(&method);
    REQUIRE(methodResolver.match({"List", "foo"}));
  }

  SECTION("Same class protected") {
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    field.declaration = &listDecl;
    listDecl.contain.addDeclareField(&field);
    local.addVariable("list", listType);
    REQUIRE(fieldResolver.match({"list", "foo"}));
  }

  SECTION("Same package protected") {
    typeLink.setPackage({"bar"});
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    field.declaration = &arrayDecl;
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);

    local.addVariable("array", arrayType);
    REQUIRE(fieldResolver.match({"array", "foo"}));
  }

  SECTION("Subclass protected") {
    Env::Field field({Env::Modifier::Protected}, Env::TypeKeyword::Integer,
                     "foo");
    field.declaration = &listDecl;
    arrayDecl.contain.addDeclareField(&field);
    arrayDecl.subType.emplace(&listDecl);
    local.addVariable("array", arrayType);
    REQUIRE(fieldResolver.match({"array", "foo"}));
  }

  SECTION("World cannot see protected") {
    Env::Field field({Env::Modifier::Static, Env::Modifier::Protected},
                     Env::TypeKeyword::Integer, "foo");
    field.declaration = &arrayDecl;
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);
    REQUIRE_FALSE(fieldResolver.match({"bar", "Array", "foo"}));
  }

  SECTION("World can see public") {
    Env::Field field({Env::Modifier::Static, Env::Modifier::Public},
                     Env::TypeKeyword::Integer, "foo");
    field.declaration = &arrayDecl;
    arrayDecl.contain.addDeclareField(&field);
    tree->update({"bar"}, arrayDecl);
    REQUIRE(fieldResolver.match({"bar", "Array", "foo"}));
  }

  SECTION("Cannot construct object of abstract class") {
    Env::TypeDeclaration baseDecl({Env::Modifier::Abstract},
                                  Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Public}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE_FALSE(constructorResolver.match(Env::Type{&baseDecl}, {}));
  }

  SECTION("World cannot use protected constructor") {
    Env::TypeDeclaration baseDecl({}, Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Protected}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE_FALSE(constructorResolver.match(Env::Type{&baseDecl}, {}));
  }

  SECTION("Found Constructor") {
    Env::TypeDeclaration baseDecl({Env::Modifier::Public},
                                  Env::DeclarationKeyword::Class, "Base");
    baseDecl.body.addConstructor({{Env::Modifier::Public}, "Base", {}});
    tree->update({"bar"}, baseDecl);
    REQUIRE(constructorResolver.match(Env::Type{&baseDecl}, {}));
  }

  SECTION("Array length") {
    REQUIRE(fieldResolver.match({Env::TypeKeyword::Integer, true}, "length"));
  }

  SECTION("Array other fields") {
    REQUIRE_FALSE(
        fieldResolver.match({Env::TypeKeyword::Integer, true}, "abs"));
  }
}
