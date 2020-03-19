#include "EnvHierarchy.hpp"
#include "ASTNode.hpp"
#include "EnvTypeLink.hpp"
#include "catch.hpp"

TEST_CASE("hierarchy validate inheritance", "[EnvHierarchy]") {
  Env::ClassHierarchy classType{
      Env::FileHeader({}, {Env::Type::Class, "canary"})};
  Env::InterfaceHierarchy interfaceType{
      Env::FileHeader({}, {Env::Type::Interface, "foo"})};

  SECTION("class extends interface") {
    REQUIRE_FALSE(classType.setExtends(&interfaceType));
  }

  SECTION("class implements class") {
    REQUIRE_FALSE(classType.addImplements(&classType));
  }

  SECTION("class extends final class") {
    Env::ClassHierarchy class2Type{Env::FileHeader(
        {AST::ModifierCode::Final}, {Env::Type::Class, "canary"})};
    REQUIRE_FALSE(classType.setExtends(&class2Type));
  }

  SECTION("class implements duplicate") {
    REQUIRE(classType.addImplements(&interfaceType));
    REQUIRE_FALSE(classType.addImplements(&interfaceType));
  }

  SECTION("interface extends class") {
    REQUIRE_FALSE(interfaceType.addExtends(&classType));
  }

  SECTION("interface extends duplicate") {
    Env::InterfaceHierarchy interface2Type{
        Env::FileHeader({}, {Env::Type::Interface, "foo"})};
    REQUIRE(interfaceType.addExtends(&interface2Type));
    REQUIRE_FALSE(interfaceType.addExtends(&interface2Type));
  }
}

TEST_CASE("builder contruct contains set", "[EnvHierarchyGraph][!hide]") {
  Env::HierarchyGraph graph;
  Env::ClassHierarchy &classType =
      graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
  Env::InterfaceHierarchy &interfaceType =
      graph.addInterface(Env::FileHeader({}, {Env::Type::Interface, "foo"}));

  SECTION("basic") {
    Env::ClassHierarchy &class2Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
    Env::ClassHierarchy &class3Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
    classType.setExtends(&class2Type);
    class2Type.setExtends(&class3Type);
    REQUIRE(graph.topologicalSort());
  }

  SECTION("class self cyclic") {
    classType.setExtends(&classType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("interface self cyclic") {
    interfaceType.addExtends(&interfaceType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("class cyclic") {
    Env::ClassHierarchy &class2Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
    Env::ClassHierarchy &class3Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
    classType.setExtends(&class2Type);
    class2Type.setExtends(&class3Type);
    class3Type.setExtends(&classType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("interface cyclic") {
    Env::InterfaceHierarchy &interface2Type =
        graph.addInterface(Env::FileHeader({}, {Env::Type::Interface, "foo"}));
    Env::InterfaceHierarchy &interface3Type =
        graph.addInterface(Env::FileHeader({}, {Env::Type::Interface, "foo"}));
    interfaceType.addExtends(&interface2Type);
    interface2Type.addExtends(&interface3Type);
    interface3Type.addExtends(&interfaceType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("class subtype interface") {
    classType.addImplements(&interfaceType);
    graph.topologicalSort();
    graph.buildSubType();
    REQUIRE(classType.subType(&interfaceType));
  }

  SECTION("no subtype default") {
    graph.topologicalSort();
    graph.buildSubType();
    REQUIRE_FALSE(classType.subType(&interfaceType));
  }
}
