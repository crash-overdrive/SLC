#include "EnvTypeContain.hpp"
#include "Config.hpp"
#include "EnvTypeDeclaration.hpp"
#include "catch.hpp"

TEST_CASE("EnvTypeContain created from Env", "[EnvTypeContain]") {
  Env::TypeContain contain;
  Env::Method method;
  method.returnType = Env::TypeKeyword::Integer;
  method.identifier = "foo";
  method.args.emplace_back(Env::TypeKeyword::Integer);
  Env::TypeDeclaration stringDecl{
      {Env::Modifier::Public}, Env::DeclarationKeyword::Class, "String"};
  Env::Type stringType{&stringDecl};

  SECTION("override signature different return") {
    Env::Method method2(method);
    method2.returnType = stringType;
    contain.addDeclare(&method2);
    REQUIRE_FALSE(contain.addDeclare(&method));
  }

  SECTION("override static method with non-static") {
    Env::Method method2(method);
    method2.modifiers = {Env::Modifier::Static};
    contain.addDeclare(&method2);
    REQUIRE_FALSE(contain.addDeclare(&method));
  }

  SECTION("override non-static method with static") {
    Env::Method method2(method);
    method.modifiers = {Env::Modifier::Static};
    contain.addDeclare(&method2);
    REQUIRE_FALSE(contain.addDeclare(&method));
  }

  SECTION("override public with protected") {
    Env::Method method2(method);
    method2.modifiers = {Env::Modifier::Public};
    method.modifiers = {Env::Modifier::Protected};
    contain.addDeclare(&method2);
    REQUIRE_FALSE(contain.addDeclare(&method));
  }

  SECTION("override protected with public") {
    Env::Method method2(method);
    method2.modifiers = {Env::Modifier::Protected};
    method.modifiers = {Env::Modifier::Public};
    contain.addDeclare(&method2);
    REQUIRE(contain.addDeclare(&method));
  }

  SECTION("override final method") {
    Env::Method method2(method);
    method2.modifiers = {Env::Modifier::Final};
    contain.addDeclare(&method2);
    REQUIRE_FALSE(contain.addDeclare(&method));
  }

  SECTION("find override") {
    auto astNode = std::make_unique<AST::MethodDeclaration>();
    method.astNode = astNode.get();
    Env::Method method2(method);
    contain.addDeclare(&method2);
    REQUIRE(contain.addDeclare(&method));

    const Env::Method *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->astNode == method.astNode);
  }

  SECTION("find inherit") {
    auto astNode2 = std::make_unique<AST::MethodDeclaration>();
    Env::Method method2;
    method2.returnType = Env::TypeKeyword::Integer;
    method2.identifier = "bar";
    method2.astNode = astNode2.get();
    method2.args.emplace_back(Env::TypeKeyword::Integer);

    contain.addDeclare(&method2);
    REQUIRE(contain.addDeclare(&method));

    const Env::Method *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->astNode == method2.astNode);
  }

  SECTION("Non-abstract in one base class") {
    Env::Method method2(method);
    Env::Method method3(method);
    method2.identifier = "bar";
    method2.modifiers.clear();
    method3.identifier = "bar";
    method3.modifiers = {Env::Modifier::Abstract};

    REQUIRE(contain.addDeclare(&method3));
    REQUIRE(contain.addDeclare(&method2));

    const Env::Method *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.empty());
  }

  SECTION("All base abstract") {
    Env::Method method2(method);
    method2.identifier = "bar";
    method2.modifiers = {Env::Modifier::Abstract};
    Env::Method method3(method);
    method3.identifier = "bar";
    method3.modifiers = {Env::Modifier::Abstract};

    contain.addDeclare(&method2);
    contain.addDeclare(&method3);
    REQUIRE(contain.addDeclare(&method));

    const Env::Method *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.find(Env::Modifier::Abstract) !=
            foundmethod->modifiers.end());
  }

  SECTION("derived abstract override base concrete") {
    Env::Method method2(method);
    method.modifiers = {Env::Modifier::Abstract};
    method2.modifiers.clear();

    contain.addDeclare(&method2);
    REQUIRE(contain.addDeclare(&method));

    const Env::Method *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.find(Env::Modifier::Abstract) !=
            foundmethod->modifiers.end());
  }

  SECTION("has abstract") {
    REQUIRE_FALSE(contain.hasAbstract());
    method.modifiers = {Env::Modifier::Abstract};
    REQUIRE(contain.addDeclare(&method));
    REQUIRE(contain.hasAbstract());
  }
}

TEST_CASE("EnvJoosContain find methods", "[EnvTypeContainFind]") {
  Env::TypeDeclaration stringDecl{
      {Env::Modifier::Public}, Env::DeclarationKeyword::Class, "String"};
  Env::Type stringType{&stringDecl, false};

  Env::TypeContain contain;
  Env::Field field{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      stringType,
      "str",
      nullptr};
  contain.addDeclare(&field);

  Env::Method method{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::TypeKeyword::Integer,
      "methodName1",
      {
          stringType,
          stringType,
          Env::TypeKeyword::Integer,
          Env::TypeKeyword::Character,
      },
      nullptr};
  contain.addDeclare(&method);

  SECTION("Field find successful") {
    std::set<Env::Modifier> modifiers = {
        Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static};
    const Env::Field *field = contain.findField("str");

    REQUIRE(field->identifier == "str");
    REQUIRE(field->type == stringType);
    REQUIRE(field->modifiers == modifiers);
  }

  SECTION("Field find unsuccessful") {
    const Env::Field *field;
    field = contain.findField("str2");
    REQUIRE(field == nullptr);
    field = contain.findField("str3");
    REQUIRE(field == nullptr);
  }

  SECTION("Method find Successful") {
    const std::set<Env::Modifier> modifiers = {
        Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static};
    std::vector<Env::Type> argList = {stringType, stringType,
                                      Env::TypeKeyword::Integer,
                                      Env::TypeKeyword::Character};
    const Env::Method *method = contain.findMethod("methodName1", argList);
    Env::Type returnType = Env::TypeKeyword::Integer;

    REQUIRE((method->identifier == "methodName1" && method->args == argList &&
             method->modifiers == modifiers &&
             method->returnType == returnType));
  }

  SECTION("Method find unsuccessful") {
    const Env::Method *method;
    // wrong identifier
    method = contain.findMethod("methodName2",
                                {
                                    stringType,
                                    stringType,
                                    Env::TypeKeyword::Integer,
                                    Env::TypeKeyword::Character,
                                }

    );
    REQUIRE(method == nullptr);

    // wrong argument datatype
    method = contain.findMethod("methodName1", {
                                                   stringType,
                                                   Env::TypeKeyword::Integer,
                                                   Env::TypeKeyword::Integer,
                                                   Env::TypeKeyword::Character,
                                               });
    REQUIRE(method == nullptr);

    // wrong argument variabletype
    method =
        contain.findMethod("methodName1", {
                                              stringType,
                                              stringType,
                                              {Env::TypeKeyword::Integer, true},
                                              Env::TypeKeyword::Character,
                                          });
    REQUIRE(method == nullptr);
  }
}
