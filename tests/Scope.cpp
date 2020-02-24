#include "Scope.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("hierarchical scopes", "[scope]"){
	Scope root{"", Scope::Type::GLOBAL, nullptr};
	root.insertChildren("foo", Scope::Type::PACKAGE);
	Scope& foo = root["foo"];
	foo.insertChildren("bar", Scope::Type::PACKAGE);
	Scope& bar = foo["bar"];
	bar.insertChildren("canary", Scope::Type::CLASS);

	SECTION("scopes can be nested") {
		REQUIRE(foo.getName() == "foo");
		REQUIRE(foo.getFullyQualifiedName() == "foo");
		REQUIRE(bar.getName() == "bar");
		REQUIRE(bar.getFullyQualifiedName() == "foo.bar");
	}

	SECTION("scopes will reject duplicates") {
		bool success = foo.insertChildren("bar", Scope::Type::CLASS);
		REQUIRE(!success);
		Scope& bar2 = foo["bar"];
		REQUIRE(bar2.contains("canary"));
		success = foo.insertChildren("bar", Scope::Type::PACKAGE);
		REQUIRE(!success);
		Scope& bar3 = foo["bar"];
		REQUIRE(bar3.contains("canary"));
		success = root.insertChildren("foo", Scope::Type::PACKAGE);
		REQUIRE(!success);
		Scope& fook = root["foo"];
		REQUIRE(fook.contains("bar"));
	}
}
