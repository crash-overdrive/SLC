#include "EnvJoosBody.hpp"
#include "Config.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("EnvJoosBody created from Env", "[EnvJoosBody]") {
  Env::JoosBody body;

  REQUIRE(body.addField(Env::JoosField{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
      "str",
      nullptr}));

  REQUIRE(body.addMethod(Env::JoosMethod{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
      "methodName1",
      {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
       Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
      nullptr}));

  SECTION("JoosField different identifier add successful") {
    REQUIRE(body.addField(Env::JoosField{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str2",
        nullptr}));
  }

  SECTION("JoosField different type add successful") {
    REQUIRE(!body.addField(Env::JoosField{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType,
                                {"Java", "Util", "Array"}},
        "str",
        nullptr}));
    REQUIRE(!body.addField(Env::JoosField{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "str",
        nullptr}));
  }

  SECTION("JoosField duplicate add un-successfull") {
    REQUIRE(!body.addField(Env::JoosField{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str",
        nullptr}));
  }

  SECTION("JoosField different VariableType add un-successfull") {
    REQUIRE(!body.addField(Env::JoosField{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"String"}},
        "str",
        nullptr}));
  }

  SECTION("JoosField different Modifier add un-successfull") {
    REQUIRE(!body.addField(Env::JoosField{
        {Env::Modifier::Public},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str",
        nullptr}));
  }

  SECTION("JoosMethod different method name - add successfull") {
    REQUIRE(body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName2",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
  }

  SECTION("JoosMethod different method name and args - add successfull") {
    REQUIRE(body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName3",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
        nullptr}));
  }

  SECTION("JoosMethod method overloading - add successfull") {
    // different order of arguments
    REQUIRE(body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}}},
        nullptr}));
    // different number of arguments
    REQUIRE(body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
        nullptr}));
    // different variable type of arguments
    REQUIRE(body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::ArrayType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
  }

  SECTION("JoosMethod method overloading - add un-successfull") {
    // different return DataType
    REQUIRE(!body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
    // different return VariableType
    REQUIRE(!body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::ArrayType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
    // different method modifiers
    REQUIRE(!body.addMethod(Env::JoosMethod{
        {Env::Modifier::Public},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
  }

  SECTION("JoosMethod same name, same args add unsuccessfull") {
    REQUIRE(!body.addMethod(Env::JoosMethod{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}},
        nullptr}));
  }

  SECTION("JoosField find Successful") {
    std::set<Env::Modifier> modifiers = {
        Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static};
    const Env::JoosField *joosField = body.findField(
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
    joosField = body.findField(
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
        "str2");
    REQUIRE(joosField == nullptr);
    joosField = body.findField(
        Env::VariableDescriptor{Env::VariableType::SimpleType, {"Array"}},
        "str");
    REQUIRE(joosField == nullptr);
    joosField = body.findField(
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
    const Env::JoosMethod *joosMethod = body.findMethod("methodName1", argList);
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
    joosMethod = body.findMethod(
        "methodName2",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);

    // wrong argument datatype
    joosMethod = body.findMethod(
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);

    // wrong argument variabletype
    joosMethod = body.findMethod(
        "methodName1",
        {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::ArrayType, {"String"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"int"}},
         Env::VariableDescriptor{Env::VariableType::SimpleType, {"char"}}});
    REQUIRE(joosMethod == nullptr);
  }
}

TEST_CASE("Visitor to create JoosType", "[JoosTypeVisitor]") {
  Client client = createClient();
  std::unique_ptr<AST::Start> root =
      client.buildAST(stdlib + "/2.0/java/lang/Integer.java");
  Env::JoosTypeVisitor visitor;
  Env::JoosBodyVisitor bodyVisitor;
  root->accept(visitor);
  root->accept(bodyVisitor);

  std::set<Env::Modifier> modifiers{
      Env::Modifier::Final,
      Env::Modifier::Public,
  };
  Env::Type type{Env::Type::Class};
  std::string identifier{"Integer"};
  std::vector<Env::JoosField> fields = {
      Env::JoosField{
          {Env::Modifier::Public},
          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
          "value",
          nullptr},
      Env::JoosField{
          {Env::Modifier::Public, Env::Modifier::Static},
          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
          "MAX_VALUE",
          nullptr}};
  std::vector<Env::JoosConstructor> constructors = {
      Env::JoosConstructor{
          {Env::Modifier::Public},
          "Integer",
          {Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}}},
          nullptr},
      Env::JoosConstructor{
          {Env::Modifier::Public},
          "Integer",
          {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
          nullptr},
      Env::JoosConstructor{{Env::Modifier::Public}, "Integer", {}, nullptr}};
  std::vector<Env::JoosMethod> methods = {
      Env::JoosMethod{
          {Env::Modifier::Public},
          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
          "intValue",
          {},
          nullptr},
      Env::JoosMethod{
          {Env::Modifier::Public, Env::Modifier::Static},
          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
          "parseInt",
          {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
          nullptr},
      Env::JoosMethod{
          {Env::Modifier::Public},
          Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
          "toString",
          {},
          nullptr}};

  REQUIRE(visitor.getModifiers() == modifiers);
  REQUIRE(visitor.getIdentifier() == identifier);
  REQUIRE(visitor.getType() == type);
  REQUIRE(bodyVisitor.getJoosFields() == fields);
  REQUIRE(bodyVisitor.getJoosConstructors() == constructors);
  REQUIRE(bodyVisitor.getJoosMethods() == methods);
}
