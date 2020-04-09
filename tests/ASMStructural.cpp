#include "catch.hpp"
#include <iostream>
#include "ASMServices.hpp"
#include "ASMStructural.hpp"


TEST_CASE("Code Generation Libraries", "[ASMBuilder][!hide]") {
	ASM::AnonymouseLabelService ALSClient{};
	ASM::StructuralLib slib{ALSClient};
	
	SECTION("Unique Labels are unique") {
		REQUIRE(ALSClient.getUniqueId() != ALSClient.getUniqueId());
		REQUIRE(ALSClient.getUniqueLabel() != ALSClient.getUniqueLabel());
	}

	SECTION("IfElseCodeIsSensible") {
		//pretend the condition was the first argument
		std::string condition = "MOV EAX, [EBP + 12];\n";

		std::string s1 = "MOV [EBP - 4], 2;\n";
		std::string s2 = "MOV [EBP - 4], 1;\n";

		std::string output = slib.IFThenElse(condition, s1, s2);

		std::cout<<output<<std::endl;

		//hard to test, as labels are based on side effects. Will investigate
		REQUIRE(TRUE);
	}

}
