#include "EnvHierarchy.hpp"
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

TEST_CASE("builder contruct contains set", "[EnvHierarchyBuilder][!hide]") {
  Env::HierarchyGraph graph;
  Env::ClassHierarchy &classType =
      graph.addClass(Env::FileHeader({}, {Env::Type::Class, "foo"}));
  Env::ClassHierarchy &interfaceType =
      graph.addClass(Env::FileHeader({}, {Env::Type::Interface, "foo"}));

  SECTION("basic") { REQUIRE(graph.topologicalSort()); }

  SECTION("class self cyclic") {
    classType.setExtends(&classType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("interface self cyclic") {
    interfaceType.setExtends(&interfaceType);
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
    Env::ClassHierarchy &interface2Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Interface, "foo"}));
    Env::ClassHierarchy &interface3Type =
        graph.addClass(Env::FileHeader({}, {Env::Type::Interface, "foo"}));
    interfaceType.setExtends(&interface2Type);
    interface2Type.setExtends(&interface3Type);
    interface3Type.setExtends(&interfaceType);
    REQUIRE_FALSE(graph.topologicalSort());
  }
}
