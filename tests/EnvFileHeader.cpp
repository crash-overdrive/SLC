#include "EnvFileHeader.hpp"
#include "Config.hpp"
#include "TestUtil.hpp"
#include "catch.hpp"

// TEST_CASE("EnvFileHeader created from AST", "[EnvFileHeader]") {
//   Env::FileHeader fileHeader =
//       Env::FileHeader({AST::ModifierCode::Public, AST::ModifierCode::Native,
//                        AST::ModifierCode::Abstract},
//                       Env::TypeDescriptor{Env::Type::Interface, "ClassName"});

//   REQUIRE(fileHeader.addField(Env::JoosField(
//       {AST::ModifierCode::Private, AST::ModifierCode::Final,
//        AST::ModifierCode::Static},
//       Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//       "str")));

//   REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//       {AST::ModifierCode::Private, AST::ModifierCode::Final,
//        AST::ModifierCode::Static},
//       Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//       "methodName1",
//       {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//        Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//        Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//        Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));

//   SECTION("JoosField different identifier add successfull") {
//     REQUIRE(fileHeader.addField(Env::JoosField(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         "str2")));
//   }

//   SECTION("JoosField different type add successfull") {
//     REQUIRE(fileHeader.addField(Env::JoosField(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "Array"),
//         "str")));
//   }

//   SECTION("JoosField duplicate add un-successfull") {
//     REQUIRE(!fileHeader.addField(Env::JoosField(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::ArrayType, "String"),
//         "str")));
//   }

//   SECTION("JoosField different VariableType add un-successfull") {
//     REQUIRE(!fileHeader.addField(Env::JoosField(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::ArrayType, "String"),
//         "str")));
//   }

//   SECTION("JoosField different Modifier add un-successfull") {
//     REQUIRE(!fileHeader.addField(Env::JoosField(
//         {AST::ModifierCode::Public},
//         Env::VariableDescriptor(Env::VariableType::ArrayType, "String"),
//         "str")));
//   }

//   SECTION("JoosMethod different method name - add successfull") {
//     REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName2",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//   }

//   SECTION("JoosMethod different method name and args - add successfull") {
//     REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName3",
//         {Env::VariableDescriptor(Env::VariableType::ArrayType, "String")})));
//   }

//   SECTION("JoosMethod method overloading - add successfull") {
//     // different order of arguments
//     REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int")})));
//     // different number of arguments
//     REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String")})));
//     // different variable type of arguments
//     REQUIRE(fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::ArrayType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//   }

//   SECTION("JoosMethod method overloading - add un-successfull") {
//     // different return DataType
//     REQUIRE(!fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//     // different return VariableType
//     REQUIRE(!fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::ArrayType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//     // different method modifiers
//     REQUIRE(!fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Public},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//   }

//   SECTION("JoosMethod same name, same args add unsuccessfull") {
//     REQUIRE(!fileHeader.addMethod(Env::JoosMethod(
//         {AST::ModifierCode::Private, AST::ModifierCode::Final,
//          AST::ModifierCode::Static},
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")})));
//   }

//   SECTION("JoosField find Successful") {
//     std::set<AST::ModifierCode> modifiers = {AST::ModifierCode::Private,
//                                              AST::ModifierCode::Final,
//                                              AST::ModifierCode::Static};
//     const Env::JoosField *joosField = fileHeader.findField(
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         "str");

//     REQUIRE(
//         (joosField->identifier == "str" &&
//          joosField->variableDescriptor ==
//              Env::VariableDescriptor(Env::VariableType::SimpleType, "String") &&
//          joosField->modifiers == modifiers));
//   }

//   SECTION("JoosField find Unsuccessful") {
//     const Env::JoosField *joosField;
//     joosField = fileHeader.findField(
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         "str2");
//     REQUIRE(joosField == nullptr);
//     joosField = fileHeader.findField(
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "Array"), "str");
//     REQUIRE(joosField == nullptr);
//     joosField = fileHeader.findField(
//         Env::VariableDescriptor(Env::VariableType::ArrayType, "String"), "str");
//     REQUIRE(joosField == nullptr);
//   }

//   SECTION("JoosMethod find Successful") {
//     const std::set<AST::ModifierCode> modifiers = {AST::ModifierCode::Private,
//                                                    AST::ModifierCode::Final,
//                                                    AST::ModifierCode::Static};
//     std::vector<Env::VariableDescriptor> argList = {
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "char")};
//     const Env::JoosMethod *joosMethod =
//         fileHeader.findMethod("methodName1", argList);
//     Env::VariableDescriptor returnType =
//         Env::VariableDescriptor(Env::VariableType::SimpleType, "int");

//     REQUIRE((joosMethod->identifier == "methodName1" &&
//              joosMethod->args == argList &&
//              joosMethod->modifiers == modifiers &&
//              joosMethod->returnType == returnType));
//   }

//   SECTION("JoosMethod find Unsuccessful") {
//     const Env::JoosMethod *joosMethod;
//     // wrong identifier
//     joosMethod = fileHeader.findMethod(
//         "methodName2",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")});
//     REQUIRE(joosMethod == nullptr);

//     // wrong argument datatype
//     joosMethod = fileHeader.findMethod(
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")});
//     REQUIRE(joosMethod == nullptr);

//     // wrong argument variabletype
//     joosMethod = fileHeader.findMethod(
//         "methodName1",
//         {Env::VariableDescriptor(Env::VariableType::SimpleType, "String"),
//          Env::VariableDescriptor(Env::VariableType::ArrayType, "String"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "int"),
//          Env::VariableDescriptor(Env::VariableType::SimpleType, "char")});
//     REQUIRE(joosMethod == nullptr);
//   }
// }

TEST_CASE("Visitor to create FileHeader", "[FileHeaderVisitor]") {
  Client client = createClient();
  std::unique_ptr<AST::Start> root =
      client.buildAST(Stdlib + "/2.0/java/lang/Integer.java");
  Env::JoosTypeVisitor Visitor;
  Env::JoosTypeBodyVisitor typeBodyVisitor;
  root->accept(Visitor);
  root->accept(typeBodyVisitor);

  std::set<AST::ModifierCode> modifiers{
      AST::ModifierCode::Final,
      AST::ModifierCode::Public,
  };
  Env::TypeDescriptor descriptor{
    Env::Type::Class,
    "Integer",
  };
  std::vector<Env::JoosField> fields = {Env::JoosField{{AST::ModifierCode::Public},
                                        Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
                                        "value", nullptr},
                                        Env::JoosField{{AST::ModifierCode::Public, AST::ModifierCode::Static},
                                        Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
                                        "MAX_VALUE", nullptr}};
  std::vector<Env::JoosConstructor> constructors = {Env::JoosConstructor{{AST::ModifierCode::Public},
                                                    "Integer", {Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}}},
                                                    nullptr},
                                                    Env::JoosConstructor{{AST::ModifierCode::Public},
                                                    "Integer", {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
                                                    nullptr},
                                                    Env::JoosConstructor{{AST::ModifierCode::Public},
                                                    "Integer", {},
                                                    nullptr}};
  std::vector<Env::JoosMethod> methods = {Env::JoosMethod{{AST::ModifierCode::Public},
                                          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
                                          "intValue", {},
                                          nullptr},
                                          Env::JoosMethod{{AST::ModifierCode::Public, AST::ModifierCode::Static},
                                          Env::VariableDescriptor{Env::VariableType::SimpleType, {"INT"}},
                                          "parseInt", {Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}}},
                                          nullptr},
                                          Env::JoosMethod{{AST::ModifierCode::Public},
                                          Env::VariableDescriptor{Env::VariableType::SimpleType, {"String"}},
                                          "toString", {},
                                          nullptr},
                                          Env::JoosMethod{{AST::ModifierCode::Public},
                                          Env::VariableDescriptor{Env::VariableType::SimpleType, {"void"}},
                                          "voidFunction", {},
                                          nullptr}};

  REQUIRE(Visitor.getModifers() == modifiers) ;
  REQUIRE(Visitor.getTypeDescriptor() == descriptor) ;
  REQUIRE(typeBodyVisitor.getJoosFields() == fields);
  REQUIRE(typeBodyVisitor.getJoosConstructors() == constructors);
  REQUIRE(typeBodyVisitor.getJoosMethods() == methods);
}
