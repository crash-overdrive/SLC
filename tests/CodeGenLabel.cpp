#include "CodeGenLabel.hpp"
#include "catch.hpp"

TEST_CASE("Code Generation Label", "[CodeGenLabel]") {
  Env::Method method({}, Env::TypeKeyword::Integer, "foo", {});
  CodeGen::LabelGenerator generator({"abc"});
  generator.generateMethod(method);
  REQUIRE(method.label == "abc.foo");
}
