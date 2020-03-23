#include "EnvJoosContain.hpp"
#include "Config.hpp"
#include "catch.hpp"

TEST_CASE("EnvJoosContain created from Env", "[EnvJoosContain]") {
  Env::JoosContain contain;
  Env::JoosBody body;
  Env::JoosMethod method;
  method.returnType = {Env::VariableType::SimpleType, {"Integer"}};
  method.identifier = "foo";
  method.args.emplace_back(Env::VariableType::SimpleType,
                           std::vector<std::string>{"Integer"});
  contain.addMethod(method);

  SECTION("override signature different return") {
    Env::JoosMethod method2(method);
    method2.returnType = {Env::VariableType::SimpleType, {"String"}};
    REQUIRE_FALSE(contain.inheritMethod(&method2));
  }

  SECTION("override static method") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Static};
    REQUIRE_FALSE(contain.inheritMethod(&method2));
  }

  SECTION("override public with protected") {
    method.modifiers = {Env::Modifier::Protected};
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Public};
    REQUIRE_FALSE(contain.inheritMethod(&method2));
  }

  SECTION("override protected with public") {
    method.modifiers = {Env::Modifier::Public};
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Protected};
    REQUIRE(contain.inheritMethod(&method2));
  }

  SECTION("override final method") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Final};
    REQUIRE_FALSE(contain.inheritMethod(&method2));
  }

  SECTION("find override") {
    auto astNode = std::make_unique<AST::MethodDeclaration>();
    method.astNode = astNode.get();
    Env::JoosMethod method2(method);
    REQUIRE(contain.inheritMethod(&method2));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->astNode == method.astNode);
  }

  SECTION("find inherit") {
    auto astNode2 = std::make_unique<AST::MethodDeclaration>();
    Env::JoosMethod method2;
    method2.returnType = {Env::VariableType::SimpleType, {"Integer"}};
    method2.identifier = "bar";
    method2.astNode = astNode2.get();
    method2.args.emplace_back(Env::VariableType::SimpleType,
                              std::vector<std::string>{"Integer"});
    REQUIRE(contain.inheritMethod(&method2));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->astNode == method2.astNode);
  }

  SECTION("has abstract") {
    REQUIRE_FALSE(contain.hasAbstract());
    method.modifiers = {Env::Modifier::Abstract};
    REQUIRE(contain.hasAbstract());
  }
}
