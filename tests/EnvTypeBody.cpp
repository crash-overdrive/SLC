#include "EnvTypeBody.hpp"
#include "Config.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

TEST_CASE("EnvTypeBody created from Env", "[EnvTypeBody]") {
  Env::TypeBody body;
  Env::TypeDeclaration stringDecl{
      {Env::Modifier::Public}, Env::DeclarationKeyword::Class, "String"};
  Env::Type stringType{&stringDecl};

  REQUIRE(body.addField(Env::Field{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      stringType,
      "str",
      nullptr}));

  REQUIRE(body.addMethod(Env::Method{
      {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
      Env::TypeKeyword::Integer,
      "methodName1",
      {
          stringType,
          stringType,
          Env::TypeKeyword::Integer,
          Env::TypeKeyword::Character,
      },
      nullptr}));

  SECTION("Field different identifier add successful") {
    REQUIRE(body.addField(Env::Field{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Boolean,
        "str2",
        nullptr}));
  }

  SECTION("Field different type add successful") {
    Env::TypeDeclaration arrayDecl{
        {Env::Modifier::Public}, Env::DeclarationKeyword::Class, "Array"};
    Env::Type arrayType{&arrayDecl};

    REQUIRE(!body.addField(Env::Field{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        arrayType,
        "str",
        nullptr}));
    REQUIRE(!body.addField(Env::Field{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "str",
        nullptr}));
  }

  SECTION("Field duplicate add unsuccessful") {
    REQUIRE(!body.addField(Env::Field{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        stringType,
        "str",
        nullptr}));
  }

  SECTION("Field different VariableType add unsuccessful") {
    REQUIRE(!body.addField(Env::Field{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        stringType,
        "str",
        nullptr}));
  }

  SECTION("Field different Modifier add unsuccessful") {
    REQUIRE(!body.addField(
        Env::Field{{Env::Modifier::Public}, stringType, "str", nullptr}));
  }

  SECTION("Method different method name - add successful") {
    REQUIRE(body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName2",
        {
            stringType,
            stringType,
            Env::TypeKeyword::Integer,
            Env::TypeKeyword::Integer,
            Env::TypeKeyword::Character,
        },
        nullptr}));
  }

  SECTION("Method different method name and args - add successful") {
    REQUIRE(body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName3",
        {
            stringType,
        },
        nullptr}));
  }

  SECTION("Method method overloading - add successful") {
    // different order of arguments
    REQUIRE(body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName1",
        {
            stringType,
            stringType,
            Env::TypeKeyword::Character,
            Env::TypeKeyword::Integer,
        },
        nullptr}));
    // different number of arguments
    REQUIRE(body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName1",
        {
            stringType,
        },
        nullptr}));
    // different variable type of arguments
    REQUIRE(body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName1",
        {
            stringType,
            stringType,
            {Env::TypeKeyword::Integer, true},
            Env::TypeKeyword::Character,
        },
        nullptr}));
  }

  SECTION("Method method overloading - add unsuccessful") {
    // different return DataType
    REQUIRE(!body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        stringType,
        "methodName1",
        {
            stringType,
            stringType,
            Env::TypeKeyword::Integer,
            Env::TypeKeyword::Character,
        },
        nullptr}));
    // different return VariableType
    REQUIRE(!body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName1",
        {
            stringType,
            stringType,
            Env::TypeKeyword::Integer,
            Env::TypeKeyword::Character,
        },
        nullptr}));
    // different method modifiers
    REQUIRE(!body.addMethod(Env::Method{{Env::Modifier::Public},
                                        Env::TypeKeyword::Integer,
                                        "methodName1",
                                        {
                                            stringType,
                                            stringType,
                                            Env::TypeKeyword::Integer,
                                            Env::TypeKeyword::Character,
                                        },
                                        nullptr}));
  }

  SECTION("Method same name, same args add unsuccessfull") {
    REQUIRE(!body.addMethod(Env::Method{
        {Env::Modifier::Protected, Env::Modifier::Final, Env::Modifier::Static},
        Env::TypeKeyword::Integer,
        "methodName1",
        {
            stringType,
            stringType,
            Env::TypeKeyword::Integer,
            Env::TypeKeyword::Character,
        },
        nullptr}));
  }
}

TEST_CASE("Visitor to create TypeBody", "[EnvTypeBodyVisitor]") {
  Client client = createClient();
  std::unique_ptr<AST::Start> root =
      client.buildAST(stdlib + "/2.0/java/io/OutputStream.java");

  Env::TypeDeclaration outputStreamDecl{
      {Env::Modifier::Public}, Env::DeclarationKeyword::Class, "OutputStream"};
  Env::TypeLink typeLink(outputStreamDecl);

  Env::TypeDeclarationVisitor visitor;
  Env::TypeBodyVisitor bodyVisitor(typeLink);
  root->accept(visitor);
  root->accept(bodyVisitor);

  std::set<Env::Modifier> modifiers{
      Env::Modifier::Public,
  };
  Env::DeclarationKeyword declarationKeyword{Env::DeclarationKeyword::Class};
  std::string identifier{"OutputStream"};
  std::vector<Env::Constructor> constructors = {
      Env::Constructor{{Env::Modifier::Public}, "OutputStream", {}, nullptr}};

  std::vector<Env::Method> methods = {
      {{Env::Modifier::Public},
       Env::TypeKeyword::Void,
       "write",
       {Env::TypeKeyword::Character},
       nullptr},
      {{Env::Modifier::Public},
       Env::TypeKeyword::Void,
       "write",
       {Env::TypeKeyword::Integer},
       nullptr},
      {{Env::Modifier::Protected, Env::Modifier::Static, Env::Modifier::Native},
       Env::TypeKeyword::Integer,
       "nativeWrite",
       {Env::TypeKeyword::Integer},
       nullptr},
      {{Env::Modifier::Public}, Env::TypeKeyword::Void, "flush", {}, nullptr},
  };

  REQUIRE(visitor.getModifiers() == modifiers);
  REQUIRE(visitor.getIdentifier() == identifier);
  REQUIRE(visitor.getKeyword() == declarationKeyword);
  REQUIRE(bodyVisitor.getConstructors() == constructors);
  REQUIRE(bodyVisitor.getMethods() == methods);
}
