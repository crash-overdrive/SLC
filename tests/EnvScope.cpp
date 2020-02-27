#include "EnvScope.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"

TEST_CASE("hierarchical scopes", "[scope]"){
	Env::Scope root{"", Env::Scope::Type::GLOBAL};
  Env::Scope *foo = root.update("foo", Env::Scope::Type::PACKAGE);
  Env::Scope *bar = foo->update("bar", Env::Scope::Type::PACKAGE);
  INFO(bar->getName());
  bar->update("canary", Env::Scope::Type::CLASS);

	SECTION("scopes will reject duplicates") {
    Env::Scope *scope;
    scope = foo->update("bar", Env::Scope::Type::CLASS);
    REQUIRE(scope == nullptr);
    scope = foo->update("bar", Env::Scope::Type::PACKAGE);
    REQUIRE(scope != nullptr);
    scope = bar->update("canary", Env::Scope::Type::PACKAGE);
    REQUIRE(scope == nullptr);
    scope = bar->update("canary", Env::Scope::Type::CLASS);
    REQUIRE(scope == nullptr);
    scope = bar->update("canary", Env::Scope::Type::INTERFACE);
    REQUIRE(scope == nullptr);
	}

  SECTION("scopes search") {
    Env::Scope *scope = root.find("foo")->find("bar")->find("canary");
    REQUIRE(scope->getType() == Env::Scope::CLASS);
  }
}
