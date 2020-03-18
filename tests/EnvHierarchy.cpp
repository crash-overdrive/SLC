#include "EnvHierarchy.hpp"
#include "EnvTypeLink.hpp"
#include "catch.hpp"

TEST_CASE("EnvHierarchy manage Class Hierarchy", "[EnvHierarchy]") {
  Env::InterfaceHierarchy interfaceType{
      Env::FileHeader({}, Env::TypeDescriptor{Env::Type::Interface, "Name"})};
  Env::ClassHierarchy classType{
      Env::FileHeader({}, Env::TypeDescriptor{Env::Type::Class, "Name"})};

  SECTION("class extends interface") {
    REQUIRE_FALSE(classType.setExtends(&interfaceType));
  }

  SECTION("class implements class") {
    REQUIRE_FALSE(classType.addImplements(&classType));
  }

  SECTION("interface extends class") {
    REQUIRE_FALSE(interfaceType.addExtends(&classType));
  }
}
