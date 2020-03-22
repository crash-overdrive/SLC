#include "EnvHierarchy.hpp"
#include "ASTNode.hpp"
#include "EnvTypeLink.hpp"
#include "catch.hpp"

TEST_CASE("hierarchy validate inheritance", "[EnvHierarchy]") {
  Env::JoosType interfaceType{{}, Env::Type::Interface, "foo"};
  Env::InterfaceHierarchy interfaceHierarchy{interfaceType};

  Env::JoosType classType{{}, Env::Type::Class, "foo"};
  Env::ClassHierarchy classHierarchy{classType};

  SECTION("class extends interface") {
    REQUIRE_FALSE(classHierarchy.setExtends(&interfaceType));
  }

  SECTION("class implements class") {
    REQUIRE_FALSE(classHierarchy.addImplements(&classType));
  }

  SECTION("class extends final class") {
    Env::JoosType classType2({Env::Modifier::Final}, Env::Type::Class,
                             "canary");
    REQUIRE_FALSE(classHierarchy.setExtends(&classType2));
  }

  SECTION("class implements duplicate") {
    REQUIRE(classHierarchy.addImplements(&interfaceType));
    REQUIRE_FALSE(classHierarchy.addImplements(&interfaceType));
  }

  SECTION("interface extends class") {
    REQUIRE_FALSE(interfaceHierarchy.addExtends(&classType));
  }

  SECTION("interface extends duplicate") {
    Env::JoosType interfaceType2({}, Env::Type::Interface, "foo");
    REQUIRE(interfaceHierarchy.addExtends(&interfaceType2));
    REQUIRE_FALSE(interfaceHierarchy.addExtends(&interfaceType2));
  }
}

TEST_CASE("builder contruct contains set", "[EnvHierarchyGraph][!hide]") {
  Env::HierarchyGraph graph;

  Env::JoosType interfaceType({}, Env::Type::Interface, "foo");
  Env::InterfaceHierarchy interfaceHierarchy(interfaceType);
  graph.addInterface(interfaceHierarchy);

  Env::JoosType classType({}, Env::Type::Class, "foo");
  Env::ClassHierarchy classHierarchy(classType);
  graph.addClass(classHierarchy);

  SECTION("basic") {
    Env::JoosType classType2({}, Env::Type::Class, "foo");
    Env::ClassHierarchy classHierarchy2(classType2);
    graph.addClass(classHierarchy2);

    Env::JoosType classType3({}, Env::Type::Class, "foo");
    Env::ClassHierarchy classHierarchy3(classType3);
    graph.addClass(classHierarchy3);

    classHierarchy.setExtends(&classType2);
    classHierarchy2.setExtends(&classType3);
    REQUIRE(graph.topologicalSort());
  }

  SECTION("class self cyclic") {
    classHierarchy.setExtends(&classType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("interface self cyclic") {
    interfaceHierarchy.addExtends(&interfaceType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("class cyclic") {
    Env::JoosType classType2({}, Env::Type::Class, "foo");
    Env::ClassHierarchy classHierarchy2(classType2);
    graph.addClass(classHierarchy2);

    Env::JoosType classType3({}, Env::Type::Class, "foo");
    Env::ClassHierarchy classHierarchy3(classType3);
    graph.addClass(classHierarchy3);

    classHierarchy.setExtends(&classType2);
    classHierarchy2.setExtends(&classType3);
    classHierarchy3.setExtends(&classType);
    REQUIRE_FALSE(graph.topologicalSort());
  }

  SECTION("interface cyclic") {
    Env::JoosType interfaceType2({}, Env::Type::Interface, "foo");
    Env::InterfaceHierarchy interfaceHierarchy2(interfaceType2);
    graph.addInterface(interfaceHierarchy2);

    Env::JoosType interfaceType3({}, Env::Type::Interface, "foo");
    Env::InterfaceHierarchy interfaceHierarchy3(interfaceType3);
    graph.addInterface(interfaceHierarchy3);

    interfaceHierarchy.addExtends(&interfaceType2);
    interfaceHierarchy2.addExtends(&interfaceType3);
    interfaceHierarchy3.addExtends(&interfaceType);
    REQUIRE_FALSE(graph.topologicalSort());
  }
}
