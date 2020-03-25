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

  SECTION("override signature different return") {
    Env::JoosMethod method2(method);
    method2.returnType = {Env::VariableType::SimpleType, {"String"}};
    contain.inheritMethod(&method2);
    REQUIRE_FALSE(contain.addDeclareMethod(&method));
  }

  SECTION("override static method with non-static") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Static};
    contain.inheritMethod(&method2);
    REQUIRE_FALSE(contain.addDeclareMethod(&method));
  }

  SECTION("override non-static method with static") {
    Env::JoosMethod method2(method);
    method.modifiers = {Env::Modifier::Static};
    contain.inheritMethod(&method2);
    REQUIRE_FALSE(contain.addDeclareMethod(&method));
  }

  SECTION("override public with protected") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Public};
    method.modifiers = {Env::Modifier::Protected};
    contain.inheritMethod(&method2);
    REQUIRE_FALSE(contain.addDeclareMethod(&method));
  }

  SECTION("override protected with public") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Protected};
    method.modifiers = {Env::Modifier::Public};
    contain.inheritMethod(&method2);
    REQUIRE(contain.addDeclareMethod(&method));
  }

  SECTION("override final method") {
    Env::JoosMethod method2(method);
    method2.modifiers = {Env::Modifier::Final};
    contain.inheritMethod(&method2);
    REQUIRE_FALSE(contain.addDeclareMethod(&method));
  }

  SECTION("find override") {
    auto astNode = std::make_unique<AST::MethodDeclaration>();
    method.astNode = astNode.get();
    Env::JoosMethod method2(method);
    contain.inheritMethod(&method2);
    REQUIRE(contain.addDeclareMethod(&method));

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

    contain.inheritMethod(&method2);
    REQUIRE(contain.addDeclareMethod(&method));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->astNode == method2.astNode);
  }

  SECTION("Non-abstract in one base class") {
    Env::JoosMethod method2(method);
    Env::JoosMethod method3(method);
    method2.identifier = "bar";
    method2.modifiers.clear();
    method3.identifier = "bar";
    method3.modifiers = {Env::Modifier::Abstract};

    contain.inheritMethod(&method2);
    contain.inheritMethod(&method3);
    REQUIRE(contain.addDeclareMethod(&method));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.empty());
  }

  SECTION("All base abstract") {
    Env::JoosMethod method2(method);
    method2.identifier = "bar";
    method2.modifiers = {Env::Modifier::Abstract};
    Env::JoosMethod method3(method);
    method3.identifier = "bar";
    method3.modifiers = {Env::Modifier::Abstract};

    contain.inheritMethod(&method2);
    contain.inheritMethod(&method3);
    REQUIRE(contain.addDeclareMethod(&method));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.find(Env::Modifier::Abstract) !=
            foundmethod->modifiers.end());
  }

  SECTION("derived abstract override base concrete") {
    Env::JoosMethod method2(method);
    method.modifiers = {Env::Modifier::Abstract};
    method2.modifiers.clear();

    contain.inheritMethod(&method2);
    REQUIRE(contain.addDeclareMethod(&method));

    const Env::JoosMethod *foundmethod =
        contain.findMethod(method2.identifier, method2.args);
    REQUIRE(foundmethod->modifiers.find(Env::Modifier::Abstract) !=
            foundmethod->modifiers.end());
  }

  SECTION("has abstract") {
    REQUIRE_FALSE(contain.hasAbstract());
    method.modifiers = {Env::Modifier::Abstract};
    REQUIRE(contain.addDeclareMethod(&method));
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
  contain.addDeclareField(&field);

  Env::JoosMethod method{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
      "methodName1",
      {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
      nullptr};
  contain.addDeclareMethod(&method);

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
