#include "EnvJoosContain.hpp"
#include "Config.hpp"
#include "catch.hpp"

TEST_CASE("EnvJoosContain created from Env", "[EnvJoosContain]") {
  Env::JoosContain contain;
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

TEST_CASE("EnvJoosContain find methods", "[EnvJoosContainFind]") {
  Env::JoosContain contain;
  Env::JoosField field{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
      "str",
      nullptr};
  contain.addField(field);

  Env::JoosMethod method{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
      "methodName1",
      {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
      nullptr};
  contain.addMethod(method);

  SECTION("JoosField find Successful") {
    std::set<Env::Modifier> modifiers = {
        Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static};
    const Env::JoosField *joosField = contain.findField(
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str");

    REQUIRE((joosField->identifier == "str" &&
             joosField->variableDescriptor ==
                 Env::VariableDescriptor{Env::VariableType::SimpleType,
                                         {"String"}} &&
             joosField->modifiers == modifiers));
  }

  SECTION("JoosField find Unsuccessful") {
    const Env::JoosField *joosField;
    joosField = contain.findField(
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str2");
    REQUIRE(joosField == nullptr);
    joosField = contain.findField(
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"Array"}},
        "str");
    REQUIRE(joosField == nullptr);
    joosField = contain.findField(
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"String"}},
        "str");
    REQUIRE(joosField == nullptr);
  }

  SECTION("JoosMethod find Successful") {
    const std::set<Env::Modifier> modifiers = {
        Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static};
    std::vector<Env::VariableDescriptor> argList = {
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}};
    const Env::JoosMethod *joosMethod =
        contain.findMethod("methodName1", argList);
    Env::VariableDescriptor returnType =
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}};

    REQUIRE((joosMethod->identifier == "methodName1" &&
             joosMethod->args == argList &&
             joosMethod->modifiers == modifiers &&
             joosMethod->returnType == returnType));
  }

  SECTION("JoosMethod find Unsuccessful") {
    const Env::JoosMethod *joosMethod;
    // wrong identifier
    joosMethod = contain.findMethod(
        "methodName2",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);

    // wrong argument datatype
    joosMethod = contain.findMethod(
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);

    // wrong argument variabletype
    joosMethod = contain.findMethod(
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::ArrayType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);
  }
}
