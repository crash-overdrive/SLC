#include "CodeGenOffset.hpp"
#include "catch.hpp"

TEST_CASE("Interface Offset Generator", "[CodeGenOffset]") {
  CodeGen::InterfaceOffsetGenerator generator;

  Env::Method method;
  method.returnType = Env::TypeKeyword::Integer;
  method.identifier = "foo";

  Env::Method method2(method);
  method2.identifier = "bar";
  Env::Method method3(method2);
  Env::Method method4(method);

  generator.update(method);
  generator.update(method2);
  generator.update(method3);
  generator.update(method4);

  REQUIRE(method.offset == 0);
  REQUIRE(method2.offset == 4);
  REQUIRE(method3.offset == 4);
  REQUIRE(method4.offset == 0);
}
