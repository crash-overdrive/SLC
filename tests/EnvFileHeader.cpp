#include "catch.hpp"

#include "EnvFileHeader.hpp"

TEST_CASE("EnvFileHeader created from AST", "[EnvFileHeader]") {
  Env::FileHeader fileHeader = Env::FileHeader("ClassName", {AST::ModifierCode::Public, AST::ModifierCode::Native, AST::ModifierCode::Abstract}, Env::Type::Interface);
  REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "String", "str")));
  REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "list")));
  REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "str")));
  REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "String", "list")));

  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "String", "int", "char"})));
  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "String", "char", "int"})));
  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String"})));
  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "char", "String", "int"})));
  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "String"})));
  REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "char", "int"})));

  SECTION("JoosField add successfull") {
    REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "String", "str2")));
    REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "list2")));
    REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "str2")));
    REQUIRE(fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "String", "list2")));
  }

  SECTION("JoosField add unsuccessfull") {
    REQUIRE(!fileHeader.addField(Env::JoosField({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "String", "str")));
    REQUIRE(!fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "list")));
    REQUIRE(!fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "List", "str")));
    REQUIRE(!fileHeader.addField(Env::JoosField({AST::ModifierCode::Public,
      AST::ModifierCode::Native, AST::ModifierCode::Abstract}, "String", "list")));
  }

  SECTION("JoosMethod add successfull") {
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array", "String", "int", "char"})));
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array", "String", "char", "int"})));
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array"})));
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array", "char", "String", "int"})));
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array", "String"})));
    REQUIRE(fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"Array", "char", "int"})));
  }

  SECTION("JoosMethod add unsuccessfull") {
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
      AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
      {"String", "String", "int", "char"})));
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"String", "String", "char", "int"})));
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"String"})));
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"String", "char", "String", "int"})));
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"String", "String"})));
    REQUIRE(!fileHeader.addMethod(Env::JoosMethod({AST::ModifierCode::Private,
        AST::ModifierCode::Final, AST::ModifierCode::Static}, "int", "methodName1",
        {"String", "char", "int"})));
  }

  SECTION("JoosField find Successful") {
    const Env::JoosField *joosField = fileHeader.findField("String", "str");
    REQUIRE((joosField->Name == "str" && joosField->Type =="String"));

    const Env::JoosField *joosField2 = fileHeader.findField("List", "list");
    REQUIRE((joosField2->Name == "list" && joosField2->Type =="List"));

    const Env::JoosField *joosField3 = fileHeader.findField("List", "str");
    REQUIRE((joosField3->Name == "str" && joosField3->Type =="List"));

    const Env::JoosField *joosField4 = fileHeader.findField("String", "list");
    REQUIRE((joosField4->Name == "list" && joosField4->Type =="String"));
  }

  SECTION("JoosField find Unsuccessful") {
    const Env::JoosField *joosField;
    joosField = fileHeader.findField("Array", "arr");
    REQUIRE(joosField == nullptr);
    joosField = fileHeader.findField("String", "string");
    REQUIRE(joosField == nullptr);
    joosField = fileHeader.findField("List", "lists");
    REQUIRE(joosField == nullptr);
    joosField = fileHeader.findField("string", "str");
    REQUIRE(joosField == nullptr);
    joosField = fileHeader.findField("list", "list");
    REQUIRE(joosField == nullptr);
  }

  SECTION("JoosMethod find Successful") {
    std::vector<std::string> argList1 = {"String", "String", "int", "char"};
    std::vector<std::string> argList2 = {"String", "String", "char", "int"};
    std::vector<std::string> argList3 = {"String"};
    std::vector<std::string> argList4 = {"String", "char", "String", "int"};
    std::vector<std::string> argList5 = {"String", "String"};
    std::vector<std::string> argList6 = {"String", "char", "int"};
    const Env::JoosMethod *joosMethod;

    joosMethod = fileHeader.findMethod("methodName1", argList1);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList1));
    joosMethod = fileHeader.findMethod("methodName1", argList2);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList2));
    joosMethod = fileHeader.findMethod("methodName1", argList3);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList3));
    joosMethod = fileHeader.findMethod("methodName1", argList4);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList4));
    joosMethod = fileHeader.findMethod("methodName1", argList5);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList5));
    joosMethod = fileHeader.findMethod("methodName1", argList6);
    REQUIRE((joosMethod->Name == "methodName1" && joosMethod->ArgType == argList6));
  }

  SECTION("JoosMethod find Unsuccessful") {
    const Env::JoosMethod *joosMethod;
    joosMethod = fileHeader.findMethod("methodName1", {"Strings", "String", "int", "char"});
    REQUIRE(joosMethod == nullptr);
    joosMethod = fileHeader.findMethod("methodName2", {"String", "String", "int", "char"});
    REQUIRE(joosMethod == nullptr);
  }
}