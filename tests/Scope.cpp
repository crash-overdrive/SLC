#include "Scope.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("hierarchical scopes", "[scope]"){
	ENV::Scope root{"", ENV::Scope::Type::GLOBAL, nullptr};
	root.insertChildren("foo", ENV::Scope::Type::PACKAGE);
	ENV::Scope& foo = root["foo"];
	foo.insertChildren("bar", ENV::Scope::Type::PACKAGE);
	ENV::Scope& bar = foo["bar"];
	bar.insertChildren("canary", ENV::Scope::Type::CLASS);

	SECTION("scopes can be nested") {
		REQUIRE(foo.getName() == "foo");
		REQUIRE(foo.getFullyQualifiedName() == "foo");
		REQUIRE(bar.getName() == "bar");
		REQUIRE(bar.getFullyQualifiedName() == "foo.bar");
	}

	SECTION("scopes will reject duplicates") {
		bool success = foo.insertChildren("bar", ENV::Scope::Type::CLASS);
		REQUIRE(!success);
		ENV::Scope& bar2 = foo["bar"];
		REQUIRE(bar2.contains("canary"));
		success = foo.insertChildren("bar", ENV::Scope::Type::PACKAGE);
		REQUIRE(!success);
		ENV::Scope& bar3 = foo["bar"];
		REQUIRE(bar3.contains("canary"));
		success = root.insertChildren("foo", ENV::Scope::Type::PACKAGE);
		REQUIRE(!success);
		ENV::Scope& fook = root["foo"];
		REQUIRE(fook.contains("bar"));
	}
}
