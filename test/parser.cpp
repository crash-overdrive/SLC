#include "catch.hpp"
#include "parser.hpp"

TEST_CASE("Done return true", "[parser]") {
	Parser Parser;

	REQUIRE(!Parser.done());
}
